<?php // -*- drupal -*-

/**
 * @file
 *  Lending API
 *
 *  The following functions are the ways we interact with the
 *  database, and do access controls.  All other functions in lending
 *  should call functions in this file and not embed SQL directly.
 */

/**
 * lending_checkout_item: checks an item out of the database
 *
 * @param $nid
 *      The node id of the item to be checked out
 * @param $uid
 *      The user id to checkout the item to
 * @param $librarian
 *      The user id of the person performing the checkout for audit reasons
 *
 * @return the success of the query
 */
function lending_checkout_item($nid, $uid, $librarian) {
  if (lending_is_checked_out($nid)) {
    // ERROR
    return;
  }
  return db_query("INSERT INTO {lending_checkouts} (nid, uid, created_lid, created_at) VALUES (%d, %d, %d, %d)", $nid, $uid, $librarian, time());
  
}



function lending_checkin_item($cid, $librarian) {
  return db_query("UPDATE {lending_checkouts} set returned_at = %d, returned_lid = %d where cid = %d", time(), $librarian, $cid);

}

function lending_current_checkout($nid) {
  $result = db_query("SELECT * from {lending_checkouts} where nid = %d and returned_at = 0", $nid);
  if ($record = db_fetch_array($result)) {
    return $record;
  }
  else {
    return 0;
  }
}

function lending_current_checkout_user($nid) {
  $result = db_query("SELECT uid from {lending_checkouts} where nid = %d and returned_at = 0", $nid);
  if ($user = db_fetch_array($result)) {
    $u = user_load($user['uid']);
    return $u;
  }
  else {
    return 0;
  }
}

function lending_is_checked_out($nid) {
  $count = db_result(db_query("SELECT count(cid) from {lending_checkouts} where nid = %d and returned_at = 0", $nid));
  return ($count > 0);
}

function lending_is_checked_out_by_user($nid, $uid) {
  $count = db_result(db_query("SELECT count(cid) from {lending_checkouts} where nid = %d and uid = %d and returned_at = 0", $nid, $uid));
  return ($count > 0);
}

function lending_delete_checkouts_by_nid($nid) {
  return db_query("DELETE from {lending_checkouts} where nid = %d", $nid);
}

/*******************************************************************
 *
 *  Request functions
 *
 */

function lending_list_request_queue($nid) {
  $requests = db_query("SELECT * from {lending_requests} where nid = %d and deleted_at = 0 and processed_at = 0", $nid);
  $rows = array();
  while($row = db_fetch_array($requests)) {
    $rows[] = $row;
  }
  return $rows;
}

function lending_request_item($nid, $uid) {
  return db_query("INSERT INTO {lending_requests} (nid, uid, created_at) VALUES (%d, %d, %d)", $nid, $uid, time());
}

function lending_delete_request($nid, $uid) {
  return db_query("UPDATE {lending_requests} set deleted_at = %d where nid = %d and uid = %d and deleted_at = 0", time(), $nid, $uid);
}

function lending_delete_request_by_rid($rid) {
  return db_query("UPDATE {lending_requests} set deleted_at = %d where rid = %d", time(), $rid);
}

function lending_has_requested($nid, $uid) {
  $count = db_result(db_query("SELECT count(rid) from {lending_requests} where nid = %d and uid = %d and deleted_at = 0 and processed_at = 0", $nid, $uid));
  return ($count > 0);
}

function lending_delete_requests_by_nid($nid) {
  return db_query("DELETE from {lending_requests} where nid = %d", $nid);
}


/**
 * Determine that this isn't a valid user to use this function.
 */

function lending_not_admin($user) {
  return ! (user_access("administer lending library", $user));
}

function lending_is_admin($user) {
  return (user_access("administer lending library", $user));
}

function lending_is_user_or_admin($user) {
  return (user_access("access lending library", $user) or user_access("administer lending library", $user));
}

function lending_not_user_or_admin($user) {
  return ! (user_access("access lending library", $user) or user_access("administer lending library", $user));
}

function lending_username($user) {
  if ($user->profile_fullname) {
    return $user->profile_fullname;
  }
  return $user->name;
}

function lending_not_acceptable_type($node) {
  $types_to_lend = variable_get('lending_node_types', array());

  // Abort if this node is not one of the types we can lend
  return !$types_to_lend[$node->type];
}
