<?php

/**
 * @file
 * Default theme implementation for a link to an issue from the [#nid] filter.
 *
 * Available variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $link: String containing the actual href link markup.
 * - $assigned_to: String containing text about who the issue is assigned to,
 *   if that information is to be printed.
 * - $status_id: The integer ID for the issue's current status.
 * - $status_label: The human-readable label of the issue's current status.
 *
 * @see template_preprocess_project_issue_issue_link()
 */
?>
<span class="<?php print $classes;?>">
<?php print $link; ?>
<?php if ($assigned_to): ?>
  <span class="project-issue-assigned-user"><?php print $assigned_to; ?></span>
<?php endif; ?>
</span>
