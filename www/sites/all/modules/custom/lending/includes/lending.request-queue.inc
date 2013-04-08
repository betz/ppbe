<?php // -*- drupal -*-

/**
 * @file
 * Functions related to display of the lending request queue
 */

function lending_show_request_queue(&$node) {
  global $user;

  $queue = lending_list_request_queue($node->nid);

  $node->content['lending_request_queue'] = array(
    '#value' => theme('lending_request_queue', $queue, $node, $user),
    '#weight' => 12
  );

}

/**
 * Define the form for entering an annotation.
 */
function lending_request_remove_form($form_state, $node) {
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

function lending_request_remove_form_validate($form, &$form_state) {
  global $user;
  if ( lending_not_user_or_admin($user) ) {
    form_set_error('submit', t('User is not allowed to check out items'));
  }

  $form_state['values']['uid'] = $user->uid;
}

function lending_request_remove_form_submit($form_id, $form_state) {
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