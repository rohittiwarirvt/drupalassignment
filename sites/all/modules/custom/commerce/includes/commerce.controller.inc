<?php

class DrupalCommerceEntityController extends DrupalDefaultEntityController implements EntityApiControllerInterface {

	protected $controllerTransaction = NULL;

	protected $lockedEntities = [];


	protected function buildQuery($ids, $conditions = [], $revision_id = FALSE) {
		$query = parent::buildQuery($ids, $conditions, $revision_id);

		if (isset($this->entityInfo['locking mode']) && $this->entityInfo['locking mode'] == 'pessimistic') {
			if (empty($this->controllerTransaction)) {
				$this->controllerTransaction = db_transaction();
			}

			$query->forUpdate();

			if (is_array($ids)) {
				$this->lockedEntities += array_flip($ids);
			}
		}
		return $query;
	}

	public function resetCache( array $ids = NULL) {
		parent::resetCache($ids);

		if (!empty($this->lockedEntities)) {
			if ( isset($ids)) {
				foreach ($ids as $id) {
					unset($this->lockedEntities[$id]);
				}
			} else {
				$this->lockedEntities = [];
			}
		}

		$this->releaseLock();
	}

	public function releaseLock() {

		if (!empty($this->entityInfo['locking mode']) && $this->entityInfo['locking mode'] == 'pessimistic') {
			if (empty($this->lockedEntities)) {
				unset($this->controllerTransaction);
			}
		}
	}

	public function invoke($hook, $entity) {
		if (!empty($this->entityInfo['fieldable']) && function_exists($function = 'field_attach' . $hook)) {
					$function($this->entityType, $entity);
		}

		module_invoke_all($this->entityType . '_' . $hook, $entity);

		if ($hook == 'presave' || 	$hook == 'insert' || $hook === 'update' || $hook == 'delete' ) {
			module_invoke_all('entity_' . $hook, $entity, $this->entityType);
		}

		if ( module_exists('rules')) {
			rules_invoke_event($this->entityType . '_' . $hook, $entity);
		}
	}

	public function save( $entity, DatabaseTransaction	$transaction = NULL) {
		if ( !isset($transaction)) {
			$transaction = db_transaction();
			$started_transaction = TRUE;
		}

		try {

			if ( !empty($entity->{$this->idKey}) ) {
				$entity->original = entity_load_unchanged($this->entityType, $entity->{$this->idKey});
			}

			$this->invoke('presave', $entity);

			if (!empty($this->revisionKey) && empty($entity->is_new) && !empty($this->revision) && !empty($entity->{$this->revisionKey})) {
				$entity->old_revision_id = $entity->{$this->revisionId};
				unset($entity->{$this->revisionKey});
			}

			if (empty($entity->{$this->idKey}) && !empty($entity->is_new)) {
				$op = 'insert';

				$return = drupal_write_record($this->entityInfo['base table'], $entity);
				if (!empty($this->revisionKey)) {
					drupal_write_record($this->entityInfo['revision table'], $entity);
					$update_base_table = TRUE;
				}
			}
			else {
				$op = 'update';
				$return = drupal_write_record($this->entityInfo['base table'], $entity);

				if (!empty($this->revisionKey)) {
					if (!empty($entity->revision)) {
						drupal_write_record($this->entityInfo('revision table'), $entity);
						$update_base_table = TRUE;
					}
					else {
						drupal_write_record($this->entityInfo['revision table'], $entity, $this->revisionKey);
					}
				}
			}

			if ($update_base_table) {
				db_update($this->entityInfo['base table'])
				 ->fields([$this->revisionKey => $entity->{$this->revisionKey}])
				 ->condition($this->idKey, $entity->{$this->idKey})
				 ->execute();
			}

			$this->entityCache[$entity->{$this->idKey}] = 	$entity;

			unset($this->lockedEntities[$entity->{$this->idKey}]);
			$this->releaseLock();
			$this->invoke($op, $entity);

			db_ignore_slave();

			unset($entity->original);
			unset($entity->is_new);
			unset($entity->revision);

			return $return;
		} catch ( Exception $e) {
			if (!empty($started_transaction)) {
				$transaction->rollback();
				watchdog_exception($this->entityType, $e);
			}
			throw $e;
		}
	}

	public function create(array $values =[]) {
		$values += ['is_new' => TRUE];

		if (isset($this->entityInfo['entity class']) && $class = $this->entityInfo['entity class']) {
			$entity = new $class($values, $this->entityType);
		} else {
			$entity = (object) $values;
		}

		drupal_alter('commerce_entity_create', $this-> entitytype, $entity);

		return $entity;
	}

	public function delete($ids, DatabaseTransaction $transaction = NULL) {
		$entities = $ids ? $this->load($ids) : FALSE;
		if(!$entities) {
			return ;
		}

		if (!isset($transaction)) {
			$transaction = db_transaction();
			$started_transaction = TRUE;
		}

		try {

			db_delete($this->entityInfo['base table'])
			 ->condition($this->idKey, array_keys($entities), 'IN')
			 ->execute();
			 if ($this->revisionKey) {
			 	db_delete($this->entityinfo['revision table'])
			 	 ->condition($this->idKey, array_keys($entities), 'IN')
				->execute();
			 }

			 $this->resetCache($ids);
			 foreach ($entities as $id => $entity) {
			 	$this->invoke('delete', $entity);
			 }

			 db_ignore_slave();

			 return TRUE;
		} catch(Exception $e) {
			if (!empty($started_transaction)) {
				$transaction->rollback();
				watchdog_exception($this->entityType, $e);
			}
			throw $e;
		}
	}

	public function export($entity, $prefix="") {
		throw new Exception("Not implemented");
	}

	public function import($export) {
		throw new Exception("Not Implemented");
	}

	public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = []) {
		$entity->content = $content;
		$langcode = isset($langcode) ? $langcode : $GLOBALS['language_content']->language;

		if (!empty($this->entityInfo['fieldable'])) {
			$entity->content += field_attach_view($this->entityType, $entity, $view_mode, $langcode);
		}

		rules_invoke_all($this->enityType . '_view', $entity, $view_mode, $langcode);
		module_invoke_all('entity_view', $entity, $this->entityType, $view_mode, $langcode);

		$build = $entity->content;
		unset($entity->content);

		return $build;
	}

	public function view($entities, $view_mode = "", $langcode = NULL, $page = NULL) {
		$rekeyed_entities = [];

		foreach ($entities as $key => $entity) {
			if (isset($entity->{$this->idKey})) {
				$key = $entity->{$this->idKey};
			}
			$rekeyed_entities[$key] = $entities;
		}

		$entities = $rekeyed_entities;

		if(!isset($this->entityInfo['view modes'][$view_mode])) {
			reset($this->entityInfo['view modes']);
			$view_mode = key($this->entityInfo['view modes']);
		}

		if (!empty($this->entityInfo['fieldable'])) {
			field_attach_preview_view($this->entityType, $entities, $view_mode);
		}

		entity_presave_view($this->entityType, $entities);
		$langcode = isset($langcode) ? $langcode : $GLOBALS['langcode_content']->language;

		$view = [];

		foreach ($entities as $key => $entity) {
			$build = entity_build_content($this->entityType, $entity, $view_mode, $langcode);

			$build += [
			 '#theme' => 'entity',
			 '#entity_type' => $this->entityType,
			 '#entity' => $entity,
			 '#view_mode' => $view_mode,
			 '#language' => $langcode,
			 '#page' => $page,
			];

			drupal_alter([$this->entityType . '_view', 'entity_view'], $build, $this->entityType);
			$view[$this->entityType][$key] = $build;
		}

		return $view;
	}
}
