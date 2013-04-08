This module is designed for a simple lending library, like one that
you might have with a club that owns dvds, books, or other equipment
that club members can check out.

The impetus for this module was making the catalog of items at the
Mid-Hudson Astronomical Association viewable online, and managing the
checkouts easily there.

== Permissions ==
There are two permissions that lending provides.  The ability to
access lending, and the ability to admin lending.

Access Lending means the following:
 * You can see who current has checked out any item
 * You can see who has current requested any item
 * You can make a request to check out an item
 * You can delete your own request to check out an item

Access Admin means the following:
 * You can check out an item for someone
 * You can check in an item for someone
 * You can delete any requests
 * You gain access to views that will let you see all the checked out
 items and all the requested items (as an expediency)

== Expected Workflow ==
(this is how we expect this to work)

Paul decides that he wants to request a DVD from the site. He logs
into his account and browses to find the DVD.  He then requests the
item.

Dave logs into the site before the club meeting and notices that Paul
has requested something. He makes sure that he brings it to the
meeting.

At the meeting Dave pulls out his iPhone and goes to the page of
requests and selects Paul and then checkout on the DVD page.  Paul now
has the item and is responsible for it.

Two months later Paul returns with the item and brings it back.  Dave
can check his item back in on the website.

== Todo ==
Documentation
 * Create file level documentation for all source files

Lifecycle
 * Delete checkouts and requests for an item when it is deleted
 * Block the deletion of a user while there are still outstanding
 checkouts on items
 * Delete requests for an item when it is still outstanding

Workflow
 * Be able to convert a request to a checkout
 * Send email on a new request to a librarian email
 * Send email on checkin / checkout to the user and the librarian for
 confirmation
 * Be able to send email to someone asking that they return their item

Usability
 * Possible more mobile friendly version of checkouts
 * Auto complete only the lendees

Theming
 * All forms should be themed, though I am finding it hard to figure
 out how to do that best and pass them arguments.
