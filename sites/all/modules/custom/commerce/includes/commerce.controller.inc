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

	public resetCache( array $ids = NULL) {
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

			if (!empty($this->revisionKey) && empty($entity->is_new) && !empty($this->revision) && !$entity->{$this->revisionKey}) {
				$entity->old_revision_id = $entity->{$this->revisionId};
				unset($entity->{$this->revisionKey});
			}

			//@todo : to be continued
		}
	}
}