<div class="node-pdf-wrapper">

  <!-- Slideshow--->

  <?php
  $title = $node->title;
  $projects = field_get_items('node', $node, 'field_executivedeck_project');
  $project_nids = array();

  if (!empty($projects)) {

    foreach ($projects as $prj) {

      if (isset($prj['target_id']) && is_numeric($prj['target_id'])) {
        $project_nids[] = $prj['target_id'];
      }
    }
  }

  $project_load = node_load_multiple($project_nids);
  ?>
  <?php if (!empty($project_load)): ?>
    <div class="slideshow-status">

      <div class="slideset">
        <!-- slide -->
        <?php foreach ($project_load as $p): ?>
          <div class="slide">
            <div class="container">
              <time class="time"
                    datetime="<?php print format_date($node->field_date_of_deck[LANGUAGE_NONE][0]['value'], 'custom', 'Y-m-d'); ?>"><?php print format_date($node->field_date_of_deck[LANGUAGE_NONE][0]['value'], 'custom', 'F Y'); ?></time>
              <h2 class="deck-title"><?php print $title; ?></h2>

              <div class="progect-ttl">
                <h2> <?php print $p->title; ?>
                  <?php
                  if ($p->field_project_manager[LANGUAGE_NONE][0]['target_id']) {
                    $project_manager_id = $p->field_project_manager[LANGUAGE_NONE][0]['target_id'];
                    $project_manager = user_load($project_manager_id);
                    $project_manager_name = !empty($project_manager->field_name[LANGUAGE_NONE][0]['value']) ? $project_manager->field_name[LANGUAGE_NONE][0]['value'] : $project_manager->name;
                    print ' - ' . $project_manager_name;
                  }
                  ?>
                </h2>
              </div>
              <section class="holder">
                <div class="text-area">
                  <div class="ttl-progect">


                    <h3>
                      <?php
                      $status_class = 'normal-status';
                      $project_status = field_get_items('node', $p, 'field_project_status');
                      if (isset($project_status[0]['tid'])) {
                        $tid = $project_status[0]['tid'];
                        $term = taxonomy_term_load($tid);

                        $status_arr = array(
                          'at risk' => 'risk',
                          'critical' => 'critical',
                          'on track' => 'default-status',
                          'launched' => 'default-status',
                        );
                        $project_status = strtolower($term->name);

                        if (isset($status_arr[$project_status])) {
                          $status_class = $status_arr[$project_status];
                        }
                      }
                      print $term->name;
                      ?>
                      |
                      <?php if (isset($p->field_quarter_for_delivery[LANGUAGE_NONE][0]['value'])): ?>
                        <?php print $p->field_quarter_for_delivery[LANGUAGE_NONE][0]['value']; ?>
                      <?php endif; ?>
                      <?php print t('DELIVERY'); ?> -
                      <?php
                      if (!empty($p->field_project_status[LANGUAGE_NONE][0]['tid'])) {
                        $project_status_id = $p->field_project_status[LANGUAGE_NONE][0]['tid'];
                        $project_status = taxonomy_term_load($project_status_id);
                        print $project_status->name;
                      }

                      ?>
                      <br><?php
                      if (isset($p->field_groups_included[LANGUAGE_NONE][0]['value'])) {
                        print $p->field_groups_included[LANGUAGE_NONE][0]['value'];
                      }
                      ?>
                    </h3>
                  </div>
                  <h3><?php print t('Short description'); ?></h3>
                  <?php print views_trim_text(array(
                    'max_length' => 250,
                    'html' => TRUE,
                  ), $p->body[LANGUAGE_NONE][0]['safe_value']); ?>
                </div>
                <div class="project-inks">
                  <strong
                    class="ttl"><?php print t('Project Links'); ?></strong>
                  <ul>
                    <li><a
                        href="<?php print url('node/' . $p->nid, array('absolute' => TRUE)); ?>">Project
                        Page</a></li>
                    <?php
                    $project_story = redhat_get_project_story($p->nid);
                    ?>
                    <?php if ($project_story): ?>
                      <li><a
                          href="<?php print url('node/' . $project_story, array('absolute' => TRUE)); ?>"><?php print t('Project Story'); ?></a>
                      </li>
                    <?php endif; ?>
                    <li><a
                        href="<?php print url('project/' . $p->nid . '/team', array('absolute' => TRUE)); ?>"><?php print t('Project
                      Team'); ?></a></li>
                    <li><a
                        href="<?php print url('project/' . $p->nid . '/blogs', array('absolute' => TRUE)); ?>"><?php print t('Project
                      Blog'); ?></a></li>

                  </ul>
                </div>
              </section>
              <div class="holder-post">

                <?php if (isset($p->field_project_objectives [LANGUAGE_NONE][0]['safe_value'])): ?>
                  <atricle class="post">
                    <h3><?php print t('PROJECT OBJECTIVES'); ?></h3>
                    <?php print $p->field_project_objectives[LANGUAGE_NONE][0]['safe_value']; ?>
                  </atricle>
                <?php endif; ?>

                <?php if (isset($p->field_accomplishments[LANGUAGE_NONE][0]['safe_value'])): ?>
                  <atricle class="post">
                    <h3><?php print t('ACCOMPLISHMENTS'); ?></h3>
                    <?php print $p->field_accomplishments[LANGUAGE_NONE][0]['safe_value']; ?>
                  </atricle>
                <?php endif; ?>

                <?php if (isset($p->field_issues_risks[LANGUAGE_NONE][0]['safe_value'])): ?>
                  <atricle class="post">
                    <h3><?php print t('ISSUES + RISKS'); ?></h3>
                    <?php print $p->field_issues_risks[LANGUAGE_NONE][0]['safe_value']; ?>
                  </atricle>
                <?php endif; ?>

              </div>
            </div>
          </div>
        <?php endforeach; ?>
        <!-- // slide -->

      </div>
      <a class="btn-prev" href="#"><?php print t('Previous'); ?></a>
      <a class="btn-next" href="#"><?php print t('Next'); ?></a>

      <div class="container">
        <div class="pagination"></div>
      </div>

    </div>
    <!-- // Slideshow -->
  <?php endif; ?>

</div>