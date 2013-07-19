<?php
/**
 * @file
 * Returns the HTML for the basic html structure of a single Drupal page.
 *
 * Complete documentation for this file is available online.
 * @see http://drupal.org/node/1728208
 */
?><!DOCTYPE html>
<!--[if IEMobile 7]><html class="iem7" <?php print $html_attributes; ?>><![endif]-->
<!--[if lte IE 6]><html class="lt-ie9 lt-ie8 lt-ie7" <?php print $html_attributes; ?>><![endif]-->
<!--[if (IE 7)&(!IEMobile)]><html class="lt-ie9 lt-ie8" <?php print $html_attributes; ?>><![endif]-->
<!--[if IE 8]><html class="lt-ie9" <?php print $html_attributes; ?>><![endif]-->
<!--[if (gte IE 9)|(gt IEMobile 7)]><!--><html <?php print $html_attributes . $rdf_namespaces; ?>><!--<![endif]-->

<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>

  <?php if ($default_mobile_metatags): ?>
    <meta name="MobileOptimized" content="width">
    <meta name="HandheldFriendly" content="true">
    <meta name="viewport" content="width=device-width">
  <?php endif; ?>
  <meta http-equiv="cleartype" content="on">

  <?php print $styles; ?>
  <?php print $scripts; ?>
  <?php if ($add_respond_js): ?>
    <!--[if lt IE 9]>
    <script src="<?php print $base_path . $path_to_zen; ?>/js/html5-respond.js"></script>
    <![endif]-->
  <?php elseif ($add_html5_shim): ?>
    <!--[if lt IE 9]>
    <script src="<?php print $base_path . $path_to_zen; ?>/js/html5.js"></script>
    <![endif]-->
  <?php endif; ?>

  <script type="text/javascript" src="http://chat.pirateparty.be/php/get.php?l=en&t=js&g=mini.xml"></script>

  <script type="text/javascript">
   jQuery(document).ready(function() {
   // Define groupchats here
   MINI_GROUPCHATS = ['parley'];
   MINI_PASSWORDS = [];

   // Define chats here
   MINI_CHATS = [];

   // Add an animation
   MINI_ANIMATE = true;

   // Define the user nickname
   MINI_NICKNAME = "Nickname098";

   // Random user nickname (if no nickname)
   MINI_RANDNICK = true;

   // Override the default session resource
   MINI_RESOURCE = "Mini";

   // Connect the user (autoconnect, show_pane, domain, username, password)
   // Notice: put true/false to autoconnect and show_pane
   launchMini(false, true, "chat.pirateparty.be");
   });
  </script>

</head>
<body class="<?php print $classes; ?>" <?php print $attributes;?>>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
</body>
</html>
