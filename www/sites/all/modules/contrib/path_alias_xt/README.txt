
DESCRIPTION
===========
This module completes the job left unfinished by Drupal's Path module.
The module extends Drupal's path alias system by automatically applying
user-friendly page aliases not only to the base URL, e.g. "about-us" (for
"node/123"), but also to any of the common extensions of the base URL, e.g.
"about-us/edit", "about-us/track", "about-us/revisions*" etc.
Similarly for the /user/.. and /taxonomy/term/.. pages.

The extended aliases may also be used when specifying visibility control for 
blocks (e.g. "about-us*"), or in fact anywhere where you are prompted to
specify pages to include or exclude.
NOTE: It is for this feature that you need to follow the INSTALLATION 
instructions below. If don't care about this feature just enable the module on
the Modules page and you're away.

Requiring no configuration, the module then makes sure that these user-friendly
(and SEO-friendly) aliases are shown everywhere, replacing their system-
generated equivalents, whether it's in the browser address bar or on
the page itself. Examples are the Edit, Track, Revisions etc. tabs, the page
statistics pages as well as any other links on your pages. No more ugly URLs
like node/123/revisons/456/view or user/7/track.

All of this makes everyone's user experience just that little more convenient
across all pages of your site.

INSTALLATION (D7)
=================
0  If you have a compiled PECL runkit extension file (see notes at the bottom
   of this file), put it in the appropriate /extension directory.
   The runkit is not mandatory, so if you don't have a runkit.so file
   (php_runkit.dll on Windows), simply continue at step 1.

   With your runkit in place proceed with either step a) or b) (preferred).

   a) In file path_alias_xt, find the line //dl('runkit.so'). Remove the
   leading double slashes.
   Or
   b) Edit your php.ini. You can use drupal page /admin/reports/status/php to
   locate the "Loaded Coniguration File". It's near the top of the page and
   usually equals something like /etc/php5/apache2/php.ini or 
   /Applications/MAMP/conf/php5.3/php.ini on a Mac.
   Add this line to the existing extension lines in your php.ini:

     extension = runkit.so

   Note: for Windows the correct line is "extension = php_runkit.dll".
   By the way, while you're down there, check that your php.ini has:

     error_reporting = E_ALL
     display_errors = On

   This ensures that if any "white screens of death" occur, these will at
   least display a clue as to what's going on.

1. As with any other module, uncompress the tar-ball, path_alias_xt.tar.gz, into
   the "sites/all/modules" subdirectory.
2. Just in case, if you are installing to a live site, put the site off-line at
   Configuration È Maintenance mode (section Development). You will only need
   about 60 seconds.
3. Visit Modules to enable the path_alias_xt module. Press "Save configuration".
   At this point your extended path aliases should have started to work for
   the majority of pages.
   However to get block visibility wildcards to work you need to complete
   either step 4a or 4b.
4a.If you have placed the runkit extension in the /extension directory and have
   edited your php.ini as per step 0b, you now restart your Drupal stack (or 
   just Apache). If you did step 0a, no restart should be necessary.
   In either case, verify the runkit has been loaded at /admin/reports/status or
   at /admin/reports/status/php.
   On the latter page, when you scroll down the page you should see a section 
   on runkit.
4b.If you did not install the runkit extension file, you need to edit file
   "inlcudes/path.inc" using any plain text editor. In this file find the
   following line (near line #235):

     function drupal_get_path_alias($path, $path_language = '') {

   Immediately below this line insert this:

       if (module_exists('path_alias_xt')) {
         return path_alias_xt_get_path_alias($path, $path_language);
       }

5. Put your site back on line at Configuration È Maintenance mode.

CONFIGURATION (D7)
==================
No further configuration is required. However, for this module to do anything
useful there need to be aliases for at least some nodes, users or taxonomy
terms. 
Authors and editors with the "Create and edit URL aliases" permission can 
manually create node aliases via the content Edit tab, near the bottom of the
page, vertical tab "URL path settings".
Or if you have the Pathauto module installed (requires Token), you can bulk-
update URL aliases, using a pattern of your choice, e.g. "content/[node:title]"
or "account/[user:name]"
See Configuration È URL aliases È Patterns.

USAGE
======
Let's say someone introduced "about-us" as an alias for some node, say the
system assigned node/123 to it. Whenever specifying page filters, for instance
on the page-specific visibility settings of a block's configuration page,
Site building È Blocks È configure, users may now type "about-us/edit",
"*about-us*" etc. rather than node/123/edit, node/123* etc.

When the system displays pages of the form "node/123/..", "about-us/.." will
be shown in the address bar instead. Plus all tabs and links on your pages are
human-readable and SEO-friendly to boot!

Bonus: it you've created an alias, say "account", for the system path "user",
then for the logged-in user their id will be suppressed, e.g "user/567/edit"
will appear as "account/edit". Nice?

Note: only URLs of the form "user/<uid>" are covered. "user/logout" will not
alias to "account/logout".

UNINSTALL
=========
You may disable path_alias_xt at any time, without reverting the change you've
made to path.inc, as that code auto-detects whether path_alias_xt is enabled or
not.
If you used the PECL runkit, you may want to put a semi-colon in front of the
"extension = runkit.so" line in your php.ini, so that the kit will no longer be
loaded.

HOW TO OBTAIN THE PECL RUNKIT EXTENSION LIBRARY
===============================================
There are some copies of runkit.so and php_runkit.dll lying about on the
internet, but most of them are old (i.e. version 0.9) and will NOT work with
PHP 5.2.x or 5.3.x. You're likely to get a white screen of death.
Check this issue for a PECL runkit library for your OS: http://drupal.org/node/760758
If there isn't one suitable for your system, you may have to compile the PECL
runkit (version 1.0 or newer) yourself, see below.

Mac/Unix/Linux
--------------
Mac users, in order for the following commands to work sign up as an Apple
Developer (free at http://developer.apple.com/programs/register), then download
and install the Xcode developer package comptabile with your OS. In addition, if
you use a MAMP stack, you may also want to point it to the Xcode header files:

$ ln -s /Developer/SDKs/MacOSX10.5.sdk/usr/include /Applications/MAMP/bin/php5.3/include

Make sure that the following commands exist on your system (try them in a 
terminal window): svn, php, phpize.
If not, you can install them with a command like the following (for Ubuntu) or
similar:

$ sudo apt-get install  subversion  php5-cli  php5-dev

Now make sure you're machine is connected to the internet. Then:

$ svn co  http://svn.php.net/repository/pecl/runkit/trunk  runkit
$ cd  runkit
$ phpize
$ ./configure
$ make

For the final step, on MAMP:
$ cp modules/runkit.so /Applications/MAMP/bin/php5.3/lib/php/extensions/no-debug-non-zts-20090626 (or similar number)
The latter directory is also where your xdebug.so lives.

Whereas on most other flavours of Unix/Linux you'd go:
$ sudo make install

This *should* place runkit.so in the correct extension directory, usually
something like /usr/lib/php5/20060613. The "extension_dir" directive
should match this by default. You can verify the active "extension_dir" on the
/admin/reports/status/php page. If it doesn't match, edit your php.ini.

Windows
-------
To compile a PECL extension on Windows see for instance: 
http://blog.renangoncalves.com/2010/01/15/how-to-compile-a-pecl-extension-on-windows
