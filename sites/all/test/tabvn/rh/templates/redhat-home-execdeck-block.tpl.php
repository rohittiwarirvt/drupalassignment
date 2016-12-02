<section class="execdeck-block">
  <span class="icon-top">icon</span>

  <div class="bg-stretch"><img
      src="<?php print base_path(); ?>sites/all/themes/redhat/images/bg-block.jpg"
      alt="image description"></div>
  <div class="container">
    <p><?php print t("The GSP team is hard at work on policy, process and system/tool projects
      that directly help the sales representative and assist Red Hat's
      transition to a solution oriented, multi-product line company"); ?>. </p>
    <strong class="ttl"><?php print t('VIEW THE'); ?>
      <br><?php print $node->title; ?></strong>
    <a href="<?php print url('node/' . $node->nid); ?>"
       class="btn"><?php print format_date($node->field_date_of_deck[LANGUAGE_NONE][0]['value'], 'custom', 'F Y'); ?>&nbsp;<?php print t('DECK'); ?></a>
  </div>
  <span class="icon-bt">icon</span>
</section>
