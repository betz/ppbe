<?php
  /**
   * @file
   * Template to display the current lendee of an item.  This template is only
   * ever reached if there is a valid checkout for an item, otherwise we never
   * get here, so assume $user and $reservation are filled out.
   *
   * $user - the lendee, a valid drupal $user object
   * $checkout - the checkout, contains all entries
   * $checkout['nid'] - node id of the checkout
   * $checkout['created_at'] - time of checkout - UNIX time
   *
   */
?>
<div class="lending-current-lender">
    <table><tr><th>Current Lendee</th><th>Checked Out</th><th></th></tr>
    <tr>
    <td><?php print lending_username($lendee); ?></td>
    <td><?php print format_date($checkout['created_at']) ?></td>
    <?php if (lending_is_admin($user)) { ?>
      <td>
        <?php print drupal_get_form('lending_checkinform', $checkout); ?>
      </td>
    <?php } ?>
    </tr>
    </table>
 </div>
