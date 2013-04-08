<?php // -*- drupal -*-

/**
 * @file
 *  Current Lender
 *
 *  This file covers all the functionality of the current lender line
 *  in the form.  That means display of the current lender as well as
 *  the checkin form that librarians can use to adjust things.
 */

function lending_show_current_lender(&$node) {
  global $user;

  $checkout = lending_current_checkout($node->nid);

  if (!$checkout) {
    return;
  }

  $node->content['current_lender'] = array(
    '#value' => theme('lending_user', user_load($checkout['uid']), $checkout),
    '#weight' => 8
  );

}

function lending_checkinform($form_state, $checkout) {
  $form['cid'] = array(
    '#type' => 'value',
    '#value' => $checkout['cid']
  );

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $checkout['nid']
  );

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $checkout['uid']
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Return Item')
  );

  return $form;
}

function lending_checkinform_validate($form, &$form_state) {
  global $user;

  if (lending_not_admin($user)) {
    form_set_error('submit', t('Not authorized to check in items'));
    return;
  }

  // TODO: should do another sanity check of cid vs. nid vs. uid
}

function lending_checkinform_submit($form_id, $form_state) {
  global $user;

  $success = lending_checkin_item(
    $form_state['values']['cid'],
    $user->uid
  );

  if ($success) {
    drupal_set_message(t('Checkin completed'));
  } else {
    drupal_set_message(t('Checkin FAILED!'));
  }
}
