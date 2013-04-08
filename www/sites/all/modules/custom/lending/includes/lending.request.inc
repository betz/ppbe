<?php // -*- drupal -*-

/**
 * @file
 * All the functions related to both the request form as well as the request
 * delete button.
 *
 */


function lending_show_request_form(&$node) {
  global $user;

  if (lending_is_checked_out_by_user($node->nid, $user->uid)) {
    // can't reserve it if we already have it checked out
    return;
  }

  if (lending_has_requested($node->nid, $user->uid)) {
    // if you already have an active request, you can't add another
    return;
  }

  $node->content['request_form'] = array(
    '#value' => drupal_get_form('lending_requestform', $node),
    '#weight' => 13
  );
}

/**
 * Define the form for entering an annotation.
 */
function lending_requestform($form_state, $node) {
  // Define a fieldset.
  $form['lending'] = array(
    '#type' => 'fieldset',
    '#title' => t('Reservations'),
  );
  // For convenience, save the node ID.
  $form['lending']['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );

  // Define a submit function.
  $form['lending']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Request this Item'),
  );
  return $form;
}

function lending_requestform_validate($form, &$form_state) {
  global $user;
  if ( lending_not_user_or_admin($user) ) {
    form_set_error('submit', t('User is not allowed to check out items'));
  }

  $form_state['values']['uid'] = $user->uid;
}

function lending_requestform_submit($form_id, $form_state) {
  $success = lending_request_item(
    $form_state['values']['nid'],
    $form_state['values']['uid']
  );

  if ($success) {
    drupal_set_message(t('Request completed'));
  } else {
    drupal_set_message(t('Request FAILED!'));
  }
}

function lending_delete_requestform($form_state, $item) {
  // For convenience, save the node ID.
  $form['rid'] = array(
    '#type' => 'hidden',
    '#value' => $item['rid'],
  );

  // Define a submit function.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete Request'),
  );
  return $form;
}

function lending_delete_requestform_submit($form_id, $form_state) {
  // Having more than one of the same form on the same page seems to cause
  // issues.  Actually track which button press came in.
  $rid = $form_state['clicked_button']['#post']['rid'];

  $success = lending_delete_request_by_rid($rid);

  if ($success) {
    drupal_set_message(t('Request completed'));
  } else {
    drupal_set_message(t('Request FAILED!'));
  }
}

function lending_checkout_requestform($form_state, $item) {

}