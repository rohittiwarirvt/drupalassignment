<div id="views-bs-accordion-<?php print $id ?>" class="<?php print $classes ?>">
  <?php foreach ($rows as $key => $row): ?>
    <div class="panel panel-default">
      <div class="panel-heading">
        <h4 class="panel-title">
          <a class="accordion-toggle <?php if(!$auto_expand_first_row[$key]): print 'collapsed'; endif;?>"
             data-toggle="collapse"
             data-parent="#views-bootstrap-accordion-<?php print $id ?>"
             href="#collapse<?php print $key ?>"
             aria-expanded="<?php if($auto_expand_first_row[$key]): print 'true'; else: print 'false'; endif;?>">
            <?php print $titles[$key] ?>
          </a>
        </h4>
      </div>

      <div id="collapse<?php print $key ?>" class="panel-collapse collapse <?php if($auto_expand_first_row[$key]): print 'in'; endif;?>"
            aria-expanded="<?php if($auto_expand_first_row[$key]): print 'true'; else: print 'false'; endif;?>">
        <div class="panel-body">
          <?php print $row ?>
        </div>
      </div>
    </div>
  <?php endforeach ?>
</div>
