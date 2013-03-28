<?php

function support_bot_admin_notification() {
  $rows = array();
  $header = array(
    array('data' => t('Actions')),
    array('data' => t('Channel'), 'field' => 'channel'),
    array('data' => t('Clients')),
    array('data' => t('States')),
    array('data' => t('Priorities')),
    array('data' => t('Active'), 'field' => 'active', 'sort' => 'desc'),
    array('data' => t('Operation')),
  );

  $query = db_select('support_bot_notification', 's')
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->orderByHeader($header);
  $query->fields('s', array('sbnid', 'channel', 'active'));
  $query->limit(25);
  $result = $query->execute();
  foreach ($result as $notification) {
    // Build action list for display.
    $actions = array();
    $result2 = db_query('SELECT action FROM {support_bot_notification_action} WHERE sbnid = :sbnid', array(':sbnid' => $notification->sbnid));
    foreach ($result2 as $action) {
      $actions[] = support_bot_admin_actions($action->action);
    }
    // Build client list for display.
    $clients = array();
    $result2 = db_query('SELECT clid FROM {support_bot_notification_client} WHERE sbnid = :sbnid', array(':sbnid' => $notification->sbnid));
    foreach ($result2 as $clid) {
      $clients[] = _support_client($clid->clid);
    }
    // Build state list for display.
    $states = array();
    $result2 = db_query('SELECT state FROM {support_bot_notification_state} WHERE sbnid = :sbnid', array(':sbnid' => $notification->sbnid));
    foreach ($result2 as $state) {
      $states[] = _support_state($state->state);
    }
    // Build priority list for display.
    $priorities = array();
    $result2 = db_query('SELECT priority FROM {support_bot_notification_priority} WHERE sbnid = :sbnid', array(':sbnid' => $notification->sbnid));
    foreach ($result2 as $priority) {
      $priorities[] = _support_priorities($priority->priority);
    }
    $rows[] = array(
      _support_bot_truncate(implode(', ', $actions)),
      _support_bot_truncate($notification->channel),
      !empty($clients) ? _support_bot_truncate(implode(', ', $clients)) : t('all'),
      !empty($states) ? _support_bot_truncate(implode(', ', $states)) : t('all'),
      !empty($priorities) ? _support_bot_truncate(implode(', ', $priorities)) : t('all'),
      $notification->active ? t('True') : t('False'),
      l(t('edit'), "admin/support/bot/notification/$notification->sbnid/edit"),
    );
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
};

function support_bot_admin_notifications($form, &$form_state, $notification = array()) {
  if (isset($notification->sbnid)) {
    $form['sbnid'] = array(
      '#type' => 'hidden',
      '#value' => $notification->sbnid,
    );
  }

  $form['actions'] = array(
    '#title' => 'Notify for ticket action(s)',
    '#type' => 'select',
    '#multiple' => TRUE,
    '#options' => support_bot_admin_actions(),
    '#required' => TRUE,
    '#description' => t('Generate notification for the selected ticket action(s).'),
    '#default_value' => isset($notification->actions) ? $notification->actions : array(),
  );

  $channels = explode(',', variable_get('bot_channels', '#test'));
  // Trim white-space that will exist if channel names entered like '#foo, #bar'
  foreach ($channels as $key => $channel) {
    // Remote white-space
    $channel = trim($channel);
    // Strip channel-keys, if any
    if ($pos = strpos($channel, ' ')) {
      $channel = substr($channel, 0, $pos);
    }
    // Strip empty channels if names have following , like '#foo, #bar, '
    if (empty($channel)) {
      unset($channels[$key]);
    }
    else {
      $channels[$key] = $channel;
    }
  }
  $keys = array_flip($channels);
  $key = isset($notification->channel) && isset($keys[$notification->channel]) ? $keys[$notification->channel] : 0;
  $form['channel'] = array(
    '#title' => 'Channel to send notification',
    '#type' => 'select',
    '#options' => $channels,
    '#required' => TRUE,
    '#description' => t('Bot will send the configured notification to the selected channel.  Available bot channels are !here.', array('!here' => l(t('configured here'), 'admin/settings/bot'))),
    '#default_value' => $key,
  );

  $clients = support_active_clients();
  $options = array();
  foreach ($clients as $client) {
    $options[$client->clid] = $client->name;
  }
  $form['clients'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Notify for selected client tickets'),
    '#options' => $options,
    '#description' => t('Optionally select one or more clients to send notifications for.  If no clients are selected, the notification will affect all clients.'),
    '#default_value' => isset($notification->clients) ? $notification->clients : array(),
  );

  $states = _support_states();
  $form['states'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Notify for tickets in selected state(s)'),
    '#options' => $states,
    '#description' => t('Optionally select one or more ticket states to send notifications for.  If no states are selected, the notification will affect all states.'),
    '#default_value' => isset($notification->states) ? $notification->states : array(),
  );
  $priorities = _support_priorities();
  $form['priorities'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Notify for tickets in selected priorities'),
    '#options' => $priorities,
    '#description' => t('Optionally select one or more ticket priorities to send notifications for.  If no priorities are selected, the notification will affect all priorities.'),
    '#default_value' => isset($notification->priorities) ? $notification->priorities : array(),
  );

  $form['active'] = array(
    '#title' => 'Notification status',
    '#type' => 'radios',
    '#options' => array(t('Disabled'), t('Enabled')),
    '#required' => TRUE,
    '#description' => t('Optionally disable notifications without deleting them.'),
    '#default_value' => isset($notification->active) ? $notification->active : 1,
  );

  if (isset($notification->sbnid)) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Update notification'),
    );
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete notification'),
    );
  }
  else {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Create notification'),
    );
  }
  return $form;
}

function support_bot_admin_notifications_submit($form, &$form_state) {
  $channels = explode(',', variable_get('bot_channels', '#test'));
  $channel = trim($channels[$form_state['values']['channel']]);
  $active = $form_state['values']['active'];
  // Strip channel-keys, if any
  if ($pos = strpos($channel, ' ')) {
    $channel = substr($channel, 0, $pos);
  }
  if (isset($form_state['values']['sbnid'])) {
    $sbnid = $form_state['values']['sbnid'];
    if ($form_state['values']['op'] == t('Delete notification')) {
      db_delete('support_bot_notification')
        ->condition('sbnid', $sbnid)
        ->execute();
      drupal_set_message('Deleted notification.');
    }
    else {
      drupal_set_message('Updated notification.');
    }
    foreach (array('action', 'client', 'state', 'priority') AS $table) {
      db_delete("support_bot_notification_$table")->condition('sbnid', $sbnid)->execute();
    }
    db_update('support_bot_notification')
      ->fields(array('channel' => $channel, 'active' => $active))
      ->condition('sbnid', $sbnid)
      ->execute();
  }
  else {
    $sbnid = db_insert('support_bot_notification')
      ->fields(array('channel' => $channel, 'active' => $active))
      ->execute();
    drupal_set_message('Created new notification.');
  }
  foreach ($form_state['values']['actions'] as $action) {
    db_insert('support_bot_notification_action')->fields(array('sbnid' => $sbnid, 'action' => $action))->execute();
  }
  foreach ($form_state['values']['clients'] as $client) {
    db_insert('support_bot_notification_client')->fields(array('sbnid' => $sbnid, 'clid' => $client))->execute();
  }
  foreach ($form_state['values']['states'] as $state) {
    db_insert('support_bot_notification_state')->fields(array('sbnid' => $sbnid, 'state' => $state))->execute();
  }
  foreach ($form_state['values']['priorities'] as $priority) {
    db_insert('support_bot_notification_priority')->fields(array('sbnid' => $sbnid, 'priority' => $priority))->execute();
  }
  drupal_goto('admin/support/bot');
}

function _support_bot_truncate($text, $length = 32, $maxmin = FALSE) {
  if (strlen($text) >= $length) {
    $text = substr($text, 0, $length) .'...';
  }
  if ($maxmin) {
    if (strlen($text) > $length) {
      while (strlen($text) > ($length - 2)) {
        $text = substr($text, 0, strlen($text) - 1);
      }
      $text .= '..';
    }
    while (strlen($text) < $length) {
      $text .= ' ';
    }
  }
  return $text;
}
