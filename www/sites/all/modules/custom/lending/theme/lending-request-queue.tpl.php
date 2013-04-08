<?php
  /**
   * @file
   * Template to display the current lendee of an item.  This template is only
   * ever reached if there is a valid checkout for an item, otherwise we never
   * get here, so assume $user and $reservation are filled out.
   *
   * $queue - the request queue
   * $node - the node in question
   * $user - the current user, so we can see if they've got requests
   *
   */
?>
<div class="lending-request-queue">
  <table><tr><th>Requested by</th><th>Request Date</th><th></th><th></th></tr>
  <?php foreach($queue as $item): ?>
  <tr>
    <td><?php print lending_username(user_load($item['uid'])); ?></td>
    <td><?php print format_date($item['created_at']) ?></td>
    <?php if ($item['uid'] == $user->uid) {?>
    <td>
       <?php print drupal_get_form('lending_delete_requestform', $item); ?>
    </td>
    <?php } else if (lending_is_admin($user)) { ?>
    <td>
          <?php print drupal_get_form('lending_delete_requestform', $item); ?>
    </td>
    <td>
        <?php print drupal_get_form('lending_checkout_requestform', $item); ?>
    </td>
    <?php } ?>

    </tr>
    <?php endforeach; ?>
    </table>
 </div>
