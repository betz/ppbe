<?php
/**
 * @file og-extras-group-info.tpl.php
 * OG Extras group info template
 *
 * Variables available:
 * - $gid: group id.
 * - $group_type: group type.
 * - $group_node: group node.
 * - $group_node_links: formatted links to create group content.
 * - $manager_uids: array of uids of group managers.
 * - $managers: array of formatted links to the group managers.
 * - $subscriber_count: number of group subscribers.
 * - $subscriber_link: formatted link with number of subscribers.
 * - $created: formatted creation date of group.
 * - $subscribe_link: formatted link to subscribe to the group.
 *
 * @ingroup views_templates
 */
?>
<?php if (!empty($gid)): ?>
  <div>
    <?php print t('Created: @date', array('@date' => $created)); ?>
  </div>
  <div>
    <?php print $subscribe_link; ?>
  </div>
<?php endif; ?>