<section class="recent-block">

  <div class="container">

    <!-- Blogs belong to this project -->


      <div class="recent-box">
        <div class="ttl-decor">
          <h3><?php print t('RECENT POSTS'); ?></h3>
        </div>

        <div class="post-holder">
          <?php $blog = views_embed_view('recent_project_blogs', 'block');?>
          <?php if (!empty($blog)): ?>
            <?php print $blog; ?>
          <?php else: ?>
            <p><?php print t('No blog content.'); ?></p>
          <?php endif; ?>

        </div>

        <a class="view"
           href="<?php print url('project/all/blogs'); ?>"><?php print t('VIEW ALL'); ?></a>
      </div>

    <!-- // BLog -->

    <!-- documents belong to this project -->

      <div class="tabs-holder">
        <ul class="tabset">
          <li class="active"><a
              href="#tab1"><?php print t('Recent Docs'); ?></a></li>
          <li class=""><a href="#tab2"><?php print t('Popular Docs'); ?></a>
          </li>
        </ul>
        <div class="tab-content">
          <div id="tab1">
            <div class="holder-post">
              <?php
              $recent_docs = views_embed_view('project_documents', 'block_recent');
              ?>
              <?php if (!empty($recent_docs)): ?>
                <?php print $recent_docs; ?>
              <?php endif; ?>
            </div>
            <a class="view-all"
               href="<?php print url('project/all/documents'); ?>"><?php print t('VIEW ALL'); ?></a>
          </div>
          <div id="tab2">
            <div class="holder-post">
              <?php
              $popular_docs = views_embed_view('project_documents', 'block_popular');
              ?>
              <?php if (!empty($popular_docs)): ?>
                <?php print $popular_docs; ?>
              <?php endif; ?>
            </div>
            <a class="view-all"
               href="<?php print url('project/all/documents'); ?>"><?php print t('VIEW ALL'); ?></a>
          </div>
        </div>
      </div>

    <!-- // Documents -->
  </div>
</section>