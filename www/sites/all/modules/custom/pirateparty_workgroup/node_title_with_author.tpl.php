<?php $account = user_load($node->uid); ?>
<div class="field-name-node_author_name">
  <span class="title">
    <h2>
      <?php print l($node->title, 'node/' . $node->nid); ?>
      <span class="author">
        <?php print t('by !name', array('!name' => l($account->name, 'user/' . $account->uid))); ?>
      </span>
    </h2>
  </span>
</div>