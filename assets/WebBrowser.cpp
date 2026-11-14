#include <webkit2/webkit2.h>
#include <gtk/gtk.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <sys/stat.h>
#include <ctime>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <pango/pango.h>

static const char* GPL3_TEXT = R"WEBBROWSER_GPL(                    GNU GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007

 Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.

                            Preamble

  The GNU General Public License is a free, copyleft license for
software and other kinds of works.

  The licenses for most software and other practical works are designed
to take away your freedom to share and change the works.  By contrast,
the GNU General Public License is intended to guarantee your freedom to
share and change all versions of a program--to make sure it remains free
software for all its users.  We, the Free Software Foundation, use the
GNU General Public License for most of our software; it applies also to
any other work released this way by its authors.  You can apply it to
your programs, too.

  When we speak of free software, we are referring to freedom, not
price.  Our General Public Licenses are designed to make sure that you
have the freedom to distribute copies of free software (and charge for
them if you wish), that you receive source code or can get it if you
want it, that you can change the software or use pieces of it in new
free programs, and that you know you can do these things.

  To protect your rights, we need to prevent others from denying you
these rights or asking you to surrender the rights.  Therefore, you have
certain responsibilities if you distribute copies of the software, or if
you modify it: responsibilities to respect the freedom of others.

  For example, if you distribute copies of such a program, whether
gratis or for a fee, you must pass on to the recipients the same
freedoms that you received.  You must make sure that they, too, receive
or can get the source code.  And you must show them these terms so they
know their rights.

  Developers that use the GNU GPL protect your rights with two steps:
(1) assert copyright on the software, and (2) offer you this License
giving you legal permission to copy, distribute and/or modify it.

  For the developers' and authors' protection, the GPL clearly explains
that there is no warranty for this free software.  For both users' and
authors' sake, the GPL requires that modified versions be marked as
changed, so that their problems will not be attributed erroneously to
authors of previous versions.

  Some devices are designed to deny users access to install or run
modified versions of the software inside them, although the manufacturer
can do so.  This is fundamentally incompatible with the aim of
protecting users' freedom to change the software.  The systematic
pattern of such abuse occurs in the area of products for individuals to
use, which is precisely where it is most unacceptable.  Therefore, we
have designed this version of the GPL to prohibit the practice for those
products.  If such problems arise substantially in other domains, we
stand ready to extend this provision to those domains in future versions
of the GPL, as needed to protect the freedom of users.

  Finally, every program is threatened constantly by software patents.
States should not allow patents to restrict development and use of
software on general-purpose computers, but in those that do, we wish to
avoid the special danger that patents applied to a free program could
make it effectively proprietary.  To prevent this, the GPL assures that
patents cannot be used to render the program non-free.

  The precise terms and conditions for copying, distribution and
modification follow.

                       TERMS AND CONDITIONS

  0. Definitions.

  "This License" refers to version 3 of the GNU General Public License.

  "Copyright" also means copyright-like laws that apply to other kinds of
works, such as semiconductor masks.

  "The Program" refers to any copyrightable work licensed under this
License.  Each licensee is addressed as "you".  "Licensees" and
"recipients" may be individuals or organizations.

  To "modify" a work means to copy from or adapt all or part of the work
in a fashion requiring copyright permission, other than the making of an
exact copy.  The resulting work is called a "modified version" of the
earlier work or a work "based on" the earlier work.

  A "covered work" means either the unmodified Program or a work based
on the Program.

  To "propagate" a work means to do anything with it that, without
permission, would make you directly or secondarily liable for
infringement under applicable copyright law, except executing it on a
computer or modifying a private copy.  Propagation includes copying,
distribution (with or without modification), making available to the
public, and in some countries other activities as well.

  To "convey" a work means any kind of propagation that enables other
parties to make or receive copies.  Mere interaction with a user through
a computer network, with no transfer of a copy, is not conveying.

  An interactive user interface displays "Appropriate Legal Notices"
to the extent that it includes a convenient and prominently visible
feature that (1) displays an appropriate copyright notice, and (2)
tells the user that there is no warranty for the work (except to the
extent that warranties are provided), that licensees may convey the
work under this License, and how to view a copy of this License.  If
the interface presents a list of user commands or options, such as a
menu, a prominent item in the list meets this criterion.

  1. Source Code.

  The "source code" for a work means the preferred form of the work
for making modifications to it.  "Object code" means any non-source
form of a work.

  A "Standard Interface" means an interface that either is an official
standard defined by a recognized standards body, or, in the case of
interfaces specified for a particular programming language, one that
is widely used among developers working in that language.

  The "System Libraries" of an executable work include anything, other
than the work as a whole, that (a) is included in the normal form of
packaging a Major Component, but which is not part of that Major
Component, and (b) serves only to enable use of the work with that
Major Component, or to implement a Standard Interface for which an
implementation is available to the public in source code form.  A
"Major Component", in this context, means a major essential component
(kernel, window system, and so on) of the specific operating system
(if any) on which the executable work runs, or a compiler used to
produce the work, or an object code interpreter used to run it.

  The "Corresponding Source" for a work in object code form means all
the source code needed to generate, install, and (for an executable
work) run the object code and to modify the work, including scripts to
control those activities.  However, it does not include the work's
System Libraries, or general-purpose tools or generally available free
programs which are used unmodified in performing those activities but
which are not part of the work.  For example, Corresponding Source
includes interface definition files associated with source files for
the work, and the source code for shared libraries and dynamically
linked subprograms that the work is specifically designed to require,
such as by intimate data communication or control flow between those
subprograms and other parts of the work.

  The Corresponding Source need not include anything that users
can regenerate automatically from other parts of the Corresponding
Source.

  The Corresponding Source for a work in source code form is that
same work.

  2. Basic Permissions.

  All rights granted under this License are granted for the term of
copyright on the Program, and are irrevocable provided the stated
conditions are met.  This License explicitly affirms your unlimited
permission to run the unmodified Program.  The output from running a
covered work is covered by this License only if the output, given its
content, constitutes a covered work.  This License acknowledges your
rights of fair use or other equivalent, as provided by copyright law.

  You may make, run and propagate covered works that you do not
convey, without conditions so long as your license otherwise remains
in force.  You may convey covered works to others for the sole purpose
of having them make modifications exclusively for you, or provide you
with facilities for running those works, provided that you comply with
the terms of this License in conveying all material for which you do
not control copyright.  Those thus making or running the covered works
for you must do so exclusively on your behalf, under your direction
and control, on terms that prohibit them from making any copies of
your copyrighted material outside their relationship with you.

  Conveying under any other circumstances is permitted solely under
the conditions stated below.  Sublicensing is not allowed; section 10
makes it unnecessary.

  3. Protecting Users' Legal Rights From Anti-Circumvention Law.

  No covered work shall be deemed part of an effective technological
measure under any applicable law fulfilling obligations under article
11 of the WIPO copyright treaty adopted on 20 December 1996, or
similar laws prohibiting or restricting circumvention of such
measures.

  When you convey a covered work, you waive any legal power to forbid
circumvention of technological measures to the extent such circumvention
is effected by exercising rights under this License with respect to
the covered work, and you disclaim any intention to limit operation or
modification of the work as a means of enforcing, against the work's
users, your or third parties' legal rights to forbid circumvention of
technological measures.

  4. Conveying Verbatim Copies.

  You may convey verbatim copies of the Program's source code as you
receive it, in any medium, provided that you conspicuously and
appropriately publish on each copy an appropriate copyright notice;
keep intact all notices stating that this License and any
non-permissive terms added in accord with section 7 apply to the code;
keep intact all notices of the absence of any warranty; and give all
recipients a copy of this License along with the Program.

  You may charge any price or no price for each copy that you convey,
and you may offer support or warranty protection for a fee.

  5. Conveying Modified Source Versions.

  You may convey a work based on the Program, or the modifications to
produce it from the Program, in the form of source code under the
terms of section 4, provided that you also meet all of these conditions:

    a) The work must carry prominent notices stating that you modified
    it, and giving a relevant date.

    b) The work must carry prominent notices stating that it is
    released under this License and any conditions added under section
    7.  This requirement modifies the requirement in section 4 to
    "keep intact all notices".

    c) You must license the entire work, as a whole, under this
    License to anyone who comes into possession of a copy.  This
    License will therefore apply, along with any applicable section 7
    additional terms, to the whole of the work, and all its parts,
    regardless of how they are packaged.  This License gives no
    permission to license the work in any other way, but it does not
    invalidate such permission if you have separately received it.

    d) If the work has interactive user interfaces, each must display
    Appropriate Legal Notices; however, if the Program has interactive
    interfaces that do not display Appropriate Legal Notices, your
    work need not make them do so.

  A compilation of a covered work with other separate and independent
works, which are not by their nature extensions of the covered work,
and which are not combined with it such as to form a larger program,
in or on a volume of a storage or distribution medium, is called an
"aggregate" if the compilation and its resulting copyright are not
used to limit the access or legal rights of the compilation's users
beyond what the individual works permit.  Inclusion of a covered work
in an aggregate does not cause this License to apply to the other
parts of the aggregate.

  6. Conveying Non-Source Forms.

  You may convey a covered work in object code form under the terms
of sections 4 and 5, provided that you also convey the
machine-readable Corresponding Source under the terms of this License,
in one of these ways:

    a) Convey the object code in, or embodied in, a physical product
    (including a physical distribution medium), accompanied by the
    Corresponding Source fixed on a durable physical medium
    customarily used for software interchange.

    b) Convey the object code in, or embodied in, a physical product
    (including a physical distribution medium), accompanied by a
    written offer, valid for at least three years and valid for as
    long as you offer spare parts or customer support for that product
    model, to give anyone who possesses the object code either (1) a
    copy of the Corresponding Source for all the software in the
    product that is covered by this License, on a durable physical
    medium customarily used for software interchange, for a price no
    more than your reasonable cost of physically performing this
    conveying of source, or (2) access to copy the
    Corresponding Source from a network server at no charge.

    c) Convey individual copies of the object code with a copy of the
    written offer to provide the Corresponding Source.  This
    alternative is allowed only occasionally and noncommercially, and
    only if you received the object code with such an offer, in accord
    with subsection 6b.

    d) Convey the object code by offering access from a designated
    place (gratis or for a charge), and offer equivalent access to the
    Corresponding Source in the same way through the same place at no
    further charge.  You need not require recipients to copy the
    Corresponding Source along with the object code.  If the place to
    copy the object code is a network server, the Corresponding Source
    may be on a different server (operated by you or a third party)
    that supports equivalent copying facilities, provided you maintain
    clear directions next to the object code saying where to find the
    Corresponding Source.  Regardless of what server hosts the
    Corresponding Source, you remain obligated to ensure that it is
    available for as long as needed to satisfy these requirements.

    e) Convey the object code using peer-to-peer transmission, provided
    you inform other peers where the object code and Corresponding
    Source of the work are being offered to the general public at no
    charge under subsection 6d.

  A separable portion of the object code, whose source code is excluded
from the Corresponding Source as a System Library, need not be
included in conveying the object code work.

  A "User Product" is either (1) a "consumer product", which means any
tangible personal property which is normally used for personal, family,
or household purposes, or (2) anything designed or sold for incorporation
into a dwelling.  In determining whether a product is a consumer product,
doubtful cases shall be resolved in favor of coverage.  For a particular
product received by a particular user, "normally used" refers to a
typical or common use of that class of product, regardless of the status
of the particular user or of the way in which the particular user
actually uses, or expects or is expected to use, the product.  A product
is a consumer product regardless of whether the product has substantial
commercial, industrial or non-consumer uses, unless such uses represent
the only significant mode of use of the product.

  "Installation Information" for a User Product means any methods,
procedures, authorization keys, or other information required to install
and execute modified versions of a covered work in that User Product from
a modified version of its Corresponding Source.  The information must
suffice to ensure that the continued functioning of the modified object
code is in no case prevented or interfered with solely because
modification has been made.

  If you convey an object code work under this section in, or with, or
specifically for use in, a User Product, and the conveying occurs as
part of a transaction in which the right of possession and use of the
User Product is transferred to the recipient in perpetuity or for a
fixed term (regardless of how the transaction is characterized), the
Corresponding Source conveyed under this section must be accompanied
by the Installation Information.  But this requirement does not apply
if neither you nor any third party retains the ability to install
modified object code on the User Product (for example, the work has
been installed in ROM).

  The requirement to provide Installation Information does not include a
requirement to continue to provide support service, warranty, or updates
for a work that has been modified or installed by the recipient, or for
the User Product in which it has been modified or installed.  Access to a
network may be denied when the modification itself materially and
adversely affects the operation of the network or violates the rules and
protocols for communication across the network.

  Corresponding Source conveyed, and Installation Information provided,
in accord with this section must be in a format that is publicly
documented (and with an implementation available to the public in
source code form), and must require no special password or key for
unpacking, reading or copying.

  7. Additional Terms.

  "Additional permissions" are terms that supplement the terms of this
License by making exceptions from one or more of its conditions.
Additional permissions that are applicable to the entire Program shall
be treated as though they were included in this License, to the extent
that they are valid under applicable law.  If additional permissions
apply only to part of the Program, that part may be used separately
under those permissions, but the entire Program remains governed by
this License without regard to the additional permissions.

  When you convey a copy of a covered work, you may at your option
remove any additional permissions from that copy, or from any part of
it.  (Additional permissions may be written to require their own
removal in certain cases when you modify the work.)  You may place
additional permissions on material, added by you to a covered work,
for which you have or can give appropriate copyright permission.

  Notwithstanding any other provision of this License, for material you
add to a covered work, you may (if authorized by the copyright holders of
that material) supplement the terms of this License with terms:

    a) Disclaiming warranty or limiting liability differently from the
    terms of sections 15 and 16 of this License; or

    b) Requiring preservation of specified reasonable legal notices or
    author attributions in that material or in the Appropriate Legal
    Notices displayed by works containing it; or

    c) Prohibiting misrepresentation of the origin of that material, or
    requiring that modified versions of such material be marked in
    reasonable ways as different from the original version; or

    d) Limiting the use for publicity purposes of names of licensors or
    authors of the material; or

    e) Declining to grant rights under trademark law for use of some
    trade names, trademarks, or service marks; or

    f) Requiring indemnification of licensors and authors of that
    material by anyone who conveys the material (or modified versions of
    it) with contractual assumptions of liability to the recipient, for
    any liability that these contractual assumptions directly impose on
    those licensors and authors.

  All other non-permissive additional terms are considered "further
restrictions" within the meaning of section 10.  If the Program as you
received it, or any part of it, contains a notice stating that it is
governed by this License along with a term that is a further
restriction, you may remove that term.  If a license document contains
a further restriction but permits relicensing or conveying under this
License, you may add to a covered work material governed by the terms
of that license document, provided that the further restriction does
not survive such relicensing or conveying.

  If you add terms to a covered work in accord with this section, you
must place, in the relevant source files, a statement of the
additional terms that apply to those files, or a notice indicating
where to find the applicable terms.

  Additional terms, permissive or non-permissive, may be stated in the
form of a separately written license, or stated as exceptions;
the above requirements apply either way.

  8. Termination.

  You may not propagate or modify a covered work except as expressly
provided under this License.  Any attempt otherwise to propagate or
modify it is void, and will automatically terminate your rights under
this License (including any patent licenses granted under the third
paragraph of section 11).

  However, if you cease all violation of this License, then your
license from a particular copyright holder is reinstated (a)
provisionally, unless and until the copyright holder explicitly and
finally terminates your license, and (b) permanently, if the copyright
holder fails to notify you of the violation by some reasonable means
prior to 60 days after the cessation.

  Moreover, your license from a particular copyright holder is
reinstated permanently if the copyright holder notifies you of the
violation by some reasonable means, this is the first time you have
received notice of violation of this License (for any work) from that
copyright holder, and you cure the violation prior to 30 days after
your receipt of the notice.

  Termination of your rights under this section does not terminate the
licenses of parties who have received copies or rights from you under
this License.  If your rights have been terminated and not permanently
reinstated, you do not qualify to receive new licenses for the same
material under section 10.

  9. Acceptance Not Required for Having Copies.

  You are not required to accept this License in order to receive or
run a copy of the Program.  Ancillary propagation of a covered work
occurring solely as a consequence of using peer-to-peer transmission
to receive a copy likewise does not require acceptance.  However,
nothing other than this License grants you permission to propagate or
modify any covered work.  These actions infringe copyright if you do
not accept this License.  Therefore, by modifying or propagating a
covered work, you indicate your acceptance of this License to do so.

  10. Automatic Licensing of Downstream Recipients.

  Each time you convey a covered work, the recipient automatically
receives a license from the original licensors, to run, modify and
propagate that work, subject to this License.  You are not responsible
for enforcing compliance by third parties with this License.

  An "entity transaction" is a transaction transferring control of an
organization, or substantially all assets of one, or subdividing an
organization, or merging organizations.  If propagation of a covered
work results from an entity transaction, each party to that
transaction who receives a copy of the work also receives whatever
licenses to the work the party's predecessor in interest had or could
give under the previous paragraph, plus a right to possession of the
Corresponding Source of the work from the predecessor in interest, if
the predecessor has it or can get it with reasonable efforts.

  You may not impose any further restrictions on the exercise of the
rights granted or affirmed under this License.  For example, you may
not impose a license fee, royalty, or other charge for exercise of
rights granted under this License, and you may not initiate litigation
(including a cross-claim or counterclaim in a lawsuit) alleging that
any patent claim is infringed by making, using, selling, offering for
sale, or importing the Program or any portion of it.

  11. Patents.

  A "contributor" is a copyright holder who authorizes use under this
License of the Program or a work on which the Program is based.  The
work thus licensed is called the contributor's "contributor version".

  A contributor's "essential patent claims" are all patent claims
owned or controlled by the contributor, whether already acquired or
hereafter acquired, that would be infringed by some manner, permitted
by this License, of making, using, or selling its contributor version,
but do not include claims that would be infringed only as a
consequence of further modification of the contributor version.  For
purposes of this definition, "control" includes the right to grant
patent sublicenses in a manner consistent with the requirements of
this License.

  Each contributor grants you a non-exclusive, worldwide, royalty-free
patent license under the contributor's essential patent claims, to
make, use, sell, offer for sale, import and otherwise run, modify and
propagate the contents of its contributor version.

  In the following three paragraphs, a "patent license" is any express
agreement or commitment, however denominated, not to enforce a patent
(such as an express permission to practice a patent or covenant not to
sue for patent infringement).  To "grant" such a patent license to a
party means to make such an agreement or commitment not to enforce a
patent against the party.

  If you convey a covered work, knowingly relying on a patent license,
and the Corresponding Source of the work is not available for anyone
to copy, free of charge and under the terms of this License, through a
publicly available network server or other readily accessible means,
then you must either (1) cause the Corresponding Source to be so
available, or (2) arrange to deprive yourself of the benefit of the
patent license for this particular work, or (3) arrange, in a manner
consistent with the requirements of this License, to extend the patent
license to downstream recipients.  "Knowingly relying" means you have
actual knowledge that, but for the patent license, your conveying the
covered work in a country, or your recipient's use of the covered work
in a country, would infringe one or more identifiable patents in that
country that you have reason to believe are valid.

  If, pursuant to or in connection with a single transaction or
arrangement, you convey, or propagate by procuring conveyance of, a
covered work, and grant a patent license to some of the parties
receiving the covered work authorizing them to use, propagate, modify
or convey a specific copy of the covered work, then the patent license
you grant is automatically extended to all recipients of the covered
work and works based on it.

  A patent license is "discriminatory" if it does not include within
the scope of its coverage, prohibits the exercise of, or is
conditioned on the non-exercise of one or more of the rights that are
specifically granted under this License.  You may not convey a covered
work if you are a party to an arrangement with a third party that is
in the business of distributing software, under which you make payment
to the third party based on the extent of your activity of conveying
the work, and under which the third party grants, to any of the
parties who would receive the covered work from you, a discriminatory
patent license (a) in connection with copies of the covered work
conveyed by you (or copies made from those copies), or (b) primarily
for and in connection with specific products or compilations that
contain the covered work, unless you entered into that arrangement,
or that patent license was granted, prior to 28 March 2007.

  Nothing in this License shall be construed as excluding or limiting
any implied license or other defenses to infringement that may
otherwise be available to you under applicable patent law.

  12. No Surrender of Others' Freedom.

  If conditions are imposed on you (whether by court order, agreement or
otherwise) that contradict the conditions of this License, they do not
excuse you from the conditions of this License.  If you cannot convey a
covered work so as to satisfy simultaneously your obligations under this
License and any other pertinent obligations, then as a consequence you may
not convey it at all.  For example, if you agree to terms that obligate you
to collect a royalty for further conveying from those to whom you convey
the Program, the only way you could satisfy both those terms and this
License would be to refrain entirely from conveying the Program.

  13. Use with the GNU Affero General Public License.

  Notwithstanding any other provision of this License, you have
permission to link or combine any covered work with a work licensed
under version 3 of the GNU Affero General Public License into a single
combined work, and to convey the resulting work.  The terms of this
License will continue to apply to the part which is the covered work,
but the special requirements of the GNU Affero General Public License,
section 13, concerning interaction through a network will apply to the
combination as such.

  14. Revised Versions of this License.

  The Free Software Foundation may publish revised and/or new versions of
the GNU General Public License from time to time.  Such new versions will
be similar in spirit to the present version, but may differ in detail to
address new problems or concerns.

  Each version is given a distinguishing version number.  If the
Program specifies that a certain numbered version of the GNU General
Public License "or any later version" applies to it, you have the
option of following the terms and conditions either of that numbered
version or of any later version published by the Free Software
Foundation.  If the Program does not specify a version number of the
GNU General Public License, you may choose any version ever published
by the Free Software Foundation.

  If the Program specifies that a proxy can decide which future
versions of the GNU General Public License can be used, that proxy's
public statement of acceptance of a version permanently authorizes you
to choose that version for the Program.

  Later license versions may give you additional or different
permissions.  However, no additional obligations are imposed on any
author or copyright holder as a result of your choosing to follow a
later version.

  15. Disclaimer of Warranty.

  THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY
APPLICABLE LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT
HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY
OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM
IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF
ALL NECESSARY SERVICING, REPAIR OR CORRECTION.

  16. Limitation of Liability.

  IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING
WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MODIFIES AND/OR CONVEYS
THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY
GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE
USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF
DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD
PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS),
EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF
SUCH DAMAGES.

  17. Interpretation of Sections 15 and 16.

  If the disclaimer of warranty and limitation of liability provided
above cannot be given local legal effect according to their terms,
reviewing courts shall apply local law that most closely approximates
an absolute waiver of all civil liability in connection with the
Program, unless a warranty or assumption of liability accompanies a
copy of the Program in return for a fee.

                     END OF TERMS AND CONDITIONS

            How to Apply These Terms to Your New Programs

  If you develop a new program, and you want it to be of the greatest
possible use to the public, the best way to achieve this is to make it
free software which everyone can redistribute and change under these terms.

  To do so, attach the following notices to the program.  It is safest
to attach them to the start of each source file to most effectively
state the exclusion of warranty; and each file should have at least
the "copyright" line and a pointer to where the full notice is found.

    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

Also add information on how to contact you by electronic and paper mail.

  If the program does terminal interaction, make it output a short
notice like this when it starts in an interactive mode:

    <program>  Copyright (C) <year>  <name of author>
    This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
    This is free software, and you are welcome to redistribute it
    under certain conditions; type `show c' for details.

The hypothetical commands `show w' and `show c' should show the appropriate
parts of the General Public License.  Of course, your program's commands
might be different; for a GUI interface, you would use an "about box".

  You should also get your employer (if you work as a programmer) or school,
if any, to sign a "copyright disclaimer" for the program, if necessary.
For more information on this, and how to apply and follow the GNU GPL, see
<https://www.gnu.org/licenses/>.

  The GNU General Public License does not permit incorporating your program
into proprietary programs.  If your program is a subroutine library, you
may consider it more useful to permit linking proprietary applications with
the library.  If this is what you want to do, use the GNU Lesser General
Public License instead of this License.  But first, please read
<https://www.gnu.org/licenses/why-not-lgpl.html>.
)WEBBROWSER_GPL";

static GtkWidget* notebook = nullptr;
static GtkWidget* url_entry = nullptr;
static GtkWidget* progress_bar = nullptr;   
static GtkWidget* status_overlay = nullptr; 
static guint status_hide_timer = 0;
static guint resize_timer_id = 0;
static gboolean is_typing = FALSE; 

static WebKitWebContext* web_context = nullptr;
static WebKitCookieManager* cookie_manager = nullptr;
static WebKitWebsiteDataManager* data_manager = nullptr;

// 独立数据目录
static std::string base_dir = "/data/data/com.termux/files/home/.config/WebBrowser/data";
static std::string history_file = base_dir + "/history/history.txt";
static std::string bookmark_file = base_dir + "/bookmarks/bookmarks.txt";
static std::string log_file = base_dir + "/logs/browser.log";

struct RecordItem {
    std::string time;
    std::string title;
    std::string url;
};
static std::vector<RecordItem> history_list;
static std::vector<RecordItem> bookmark_list;

static GtkListStore* history_store = nullptr;
static GtkListStore* cookie_store = nullptr;
static GtkListStore* bookmark_store = nullptr;
static GtkListStore* log_store = nullptr;
static GtkWidget* record_dialog = nullptr;
static GtkWidget* record_notebook = nullptr;
static GtkWidget* record_multi_button = nullptr;
static GtkWidget* record_select_all_button = nullptr;
static GtkWidget* record_invert_button = nullptr;
static GtkWidget* record_delete_button = nullptr;
static GtkWidget* record_copy_button = nullptr;
static bool record_multi_mode = false;
static std::vector<GtkTreeViewColumn*> record_selection_columns;
static std::vector<std::string> log_lines;
static std::string settings_file = base_dir + "/settings.conf";
static std::string ui_theme = "light";
static std::string ui_language = "zh";
static GtkCssProvider* app_css = nullptr;
static bool app_css_installed = false;
static GtkWidget* main_window = nullptr;
static GtkWidget* btn_back = nullptr;
static GtkWidget* btn_forward = nullptr;
static GtkWidget* btn_refresh = nullptr;
static GtkWidget* btn_history = nullptr;
static GtkWidget* btn_bookmark = nullptr;
static GtkWidget* btn_settings = nullptr;
static GtkWidget* tab_add_button = nullptr;
static GtkWidget* tab_close_button = nullptr;
static bool javascript_enabled = true;
static bool webgl_enabled = true;
static bool media_requires_gesture = true;
static double page_zoom_percent = 50.0; // 默认使用原版自适应缩放比例的 50%
static bool zoom_setting_version_seen = false;

GtkWidget* get_webview_from_page(GtkWidget* page) {
    if (!page) return nullptr;
    GList* children = gtk_container_get_children(GTK_CONTAINER(page));
    if (children) {
        GtkWidget* webview = GTK_WIDGET(children->data);
        g_list_free(children);
        return webview;
    }
    return nullptr;
}

GtkWidget* get_current_webview() {
    int page_idx = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
    if (page_idx == -1) return nullptr;
    GtkWidget* page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), page_idx);
    return get_webview_from_page(page);
}

void write_log(const std::string& msg) {
    FILE* f = fopen(log_file.c_str(), "a");
    if (f) {
        time_t now = time(0);
        char time_buf[64];
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        fprintf(f, "[%s] %s\n", time_buf, msg.c_str());
        fclose(f);
    }
}

const char* tr(const char* zh, const char* en);

gboolean hide_status_callback(gpointer user_data) {
    gtk_widget_hide(status_overlay);
    status_hide_timer = 0;
    return G_SOURCE_REMOVE;
}

void update_status_text(const char* text, gboolean auto_hide) {
    if (!status_overlay) return;
    gtk_label_set_text(GTK_LABEL(status_overlay), text);
    gtk_widget_show(status_overlay);
    if (status_hide_timer > 0) {
        g_source_remove(status_hide_timer);
        status_hide_timer = 0;
    }
    if (auto_hide) status_hide_timer = g_timeout_add(1600, hide_status_callback, NULL);
}

void on_progress_changed(WebKitWebView* web_view, GParamSpec* pspec, gpointer user_data) {
    if (GTK_WIDGET(web_view) == get_current_webview() && progress_bar &&
        webkit_web_view_is_loading(web_view)) {
        double real_progress = webkit_web_view_get_estimated_load_progress(web_view);
        gtk_widget_show(progress_bar);
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), real_progress);
        if (status_overlay && !gtk_widget_get_visible(status_overlay))
            update_status_text(tr("正在加载...", "Loading..."), FALSE);
    }
}

std::string resolve_input(const char* input) {
    std::string s(input);
    if (s.empty()) return "https://cn.bing.com";
    if (s.find(" ") == std::string::npos && s.find(".") != std::string::npos) {
        if (s.find("://") == std::string::npos) return "https://" + s;
        return s;
    }
    return "https://cn.bing.com/search?q=" + s;
}

void read_records(const std::string& file, std::vector<RecordItem>& list) {
    list.clear();
    FILE* f = fopen(file.c_str(), "r");
    if (f) {
        char buf[4096];
        while (fgets(buf, sizeof(buf), f)) {
            std::string line(buf);
            if (!line.empty() && line.back() == '\n') line.pop_back();
            size_t pos1 = line.find('|');
            size_t pos2 = line.find('|', pos1 + 1);
            if (pos1 != std::string::npos && pos2 != std::string::npos) {
                list.push_back({ line.substr(0, pos1), line.substr(pos1 + 1, pos2 - pos1 - 1), line.substr(pos2 + 1) });
            }
        }
        fclose(f);
    }
}

void write_records(const std::string& file, const std::vector<RecordItem>& list) {
    FILE* f = fopen(file.c_str(), "w");
    if (f) {
        for (const auto& item : list) {
            fprintf(f, "%s|%s|%s\n", item.time.c_str(), item.title.c_str(), item.url.c_str());
        }
        fclose(f);
    }
}

void save_history(const char* url, const char* title) {
    if (!url || strstr(url, "about:blank") || strstr(url, "data:text/html")) return;
    std::string t = (title && strlen(title) > 0) ? title : url;
    for (char& c : t) { if (c == '|' || c == '\n' || c == '\r') c = ' '; }
    time_t now = time(0);
    char time_buf[64];
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    history_list.push_back({time_buf, t, url});
    write_records(history_file, history_list);
}

void add_tab(const char* url);

gboolean on_decide_policy(WebKitWebView* web_view, WebKitPolicyDecision* decision, WebKitPolicyDecisionType type, gpointer user_data) {
    if (type == WEBKIT_POLICY_DECISION_TYPE_NEW_WINDOW_ACTION) {
        WebKitNavigationPolicyDecision* nav_decision = WEBKIT_NAVIGATION_POLICY_DECISION(decision);
        WebKitNavigationAction* action = webkit_navigation_policy_decision_get_navigation_action(nav_decision);
        WebKitURIRequest* request = webkit_navigation_action_get_request(action);
        const gchar* uri = webkit_uri_request_get_uri(request);

        if (uri) {
            webkit_policy_decision_ignore(decision);
            add_tab(uri);
            return TRUE;
        }
    }
    return FALSE;
}


enum RecordColumns { REC_TEXT = 0, REC_URL = 1, REC_TIME = 2, REC_SELECTED = 3, REC_INDEX = 4 };

bool is_english() { return ui_language == "en"; }
const char* tr(const char* zh, const char* en) { return is_english() ? en : zh; }

void apply_app_theme() {
    if (!app_css) app_css = gtk_css_provider_new();
    const char* css_light =
        "window, dialog { background-color: #f4f6fa; color: #20242b; }"
        "button { background-image: none; background-color: #ffffff; color: #20242b; border: 1px solid #cbd2dc; border-radius: 8px; padding: 6px 11px; }"
        "button:hover { background-color: #e8eef8; }"
        "button:active { background-color: #dce5f2; }"
        "entry, textview, text, treeview { background-color: #ffffff; color: #20242b; }"
        "notebook { background-color: #f4f6fa; }"
        "notebook header { background-color: #e9edf4; }"
        "notebook tab { background-color: #e9edf4; color: #20242b; padding: 7px 12px; }"
        "notebook tab:checked { background-color: #ffffff; color: #20242b; }"
        "checkbutton, radiobutton, label { color: #20242b; }"
        "frame { border-color: #d5dce6; }"
        "scrolledwindow { background-color: #f4f6fa; }"
        "scale trough { background-color: #d5dce6; }";
    const char* css_dark =
        "window, dialog { background-color: #202630; color: #edf0f5; }"
        "button { background-image: none; background-color: #303946; color: #edf0f5; border: 1px solid #4b5869; border-radius: 8px; padding: 6px 11px; }"
        "button:hover { background-color: #3d4a5b; }"
        "button:active { background-color: #46566a; }"
        "entry, textview, text, treeview { background-color: #29313d; color: #edf0f5; }"
        "notebook { background-color: #202630; }"
        "notebook header { background-color: #29313d; }"
        "notebook tab { background-color: #303946; color: #edf0f5; padding: 7px 12px; }"
        "notebook tab:checked { background-color: #202630; color: #edf0f5; }"
        "checkbutton, radiobutton, label { color: #edf0f5; }"
        "frame { border-color: #465263; }"
        "scrolledwindow { background-color: #202630; }"
        "scale trough { background-color: #465263; }";
    gtk_css_provider_load_from_data(app_css, ui_theme == "dark" ? css_dark : css_light, -1, NULL);
    GdkScreen* screen = gdk_screen_get_default();
    if (screen && !app_css_installed) {
        gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(app_css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        app_css_installed = true;
    }
    GtkSettings* settings = gtk_settings_get_default();
    if (settings) g_object_set(settings, "gtk-application-prefer-dark-theme", ui_theme == "dark", NULL);
    if (main_window) gtk_widget_queue_draw(main_window);
}

void apply_ui_language() {
    if (btn_back) gtk_widget_set_tooltip_text(btn_back, tr("后退", "Back"));
    if (btn_forward) gtk_widget_set_tooltip_text(btn_forward, tr("前进", "Forward"));
    if (btn_refresh) gtk_widget_set_tooltip_text(btn_refresh, tr("刷新", "Reload"));
    if (btn_history) gtk_widget_set_tooltip_text(btn_history, tr("历史记录", "History"));
    if (btn_bookmark) gtk_widget_set_tooltip_text(btn_bookmark, tr("收藏夹", "Bookmarks"));
    if (btn_settings) gtk_widget_set_tooltip_text(btn_settings, tr("设置", "Settings"));
    if (url_entry) gtk_entry_set_placeholder_text(GTK_ENTRY(url_entry), tr("输入网址或搜索内容", "Enter a URL or search"));
    if (tab_add_button) gtk_widget_set_tooltip_text(tab_add_button, tr("新建标签页", "New tab"));
    if (tab_close_button) gtk_widget_set_tooltip_text(tab_close_button, tr("关闭当前标签页", "Close current tab"));
    if (main_window) gtk_window_set_title(GTK_WINDOW(main_window), "WebBrowser");
    if (web_context) {
        const char* zh_languages[] = {"zh-CN", "zh", "en", NULL};
        const char* en_languages[] = {"en", "zh-CN", "zh", NULL};
        webkit_web_context_set_preferred_languages(web_context, is_english() ? en_languages : zh_languages);
    }
}

void load_settings() {
    FILE* f = fopen(settings_file.c_str(), "r");
    if (!f) return;
    char key[128], value[256];
    while (fscanf(f, "%127[^=]=%255s\n", key, value) == 2) {
        if (strcmp(key, "theme") == 0 && (strcmp(value, "light") == 0 || strcmp(value, "dark") == 0)) ui_theme = value;
        else if (strcmp(key, "language") == 0 && (strcmp(value, "zh") == 0 || strcmp(value, "en") == 0)) ui_language = value;
        else if (strcmp(key, "javascript_enabled") == 0) javascript_enabled = strcmp(value, "1") == 0;
        else if (strcmp(key, "webgl_enabled") == 0) webgl_enabled = strcmp(value, "1") == 0;
        else if (strcmp(key, "media_requires_gesture") == 0) media_requires_gesture = strcmp(value, "1") == 0;
        else if (strcmp(key, "page_zoom_percent") == 0) page_zoom_percent = std::max(0.0, std::min(100.0, atof(value)));
        else if (strcmp(key, "zoom_setting_version") == 0) zoom_setting_version_seen = (strcmp(value, "2") == 0 || strcmp(value, "3") == 0);
    }
    fclose(f);
    if (!zoom_setting_version_seen) page_zoom_percent = 50.0;
}

void save_settings() {
    FILE* f = fopen(settings_file.c_str(), "w");
    if (!f) return;
    fprintf(f, "theme=%s\nlanguage=%s\njavascript_enabled=%d\nwebgl_enabled=%d\nmedia_requires_gesture=%d\npage_zoom_percent=%.0f\nzoom_setting_version=3\n",
        ui_theme.c_str(), ui_language.c_str(), javascript_enabled ? 1 : 0,
        webgl_enabled ? 1 : 0, media_requires_gesture ? 1 : 0, page_zoom_percent);
    fclose(f);
}

void refresh_history_ui() {
    if (!history_store) return;
    gtk_list_store_clear(history_store);
    for (int i = (int)history_list.size() - 1; i >= 0; --i) {
        GtkTreeIter iter;
        gtk_list_store_append(history_store, &iter);
        gtk_list_store_set(history_store, &iter, REC_TEXT, history_list[i].title.c_str(),
            REC_URL, history_list[i].url.c_str(), REC_TIME, history_list[i].time.c_str(),
            REC_SELECTED, FALSE, REC_INDEX, i, -1);
    }
}

void refresh_bookmark_ui() {
    if (!bookmark_store) return;
    gtk_list_store_clear(bookmark_store);
    for (int i = (int)bookmark_list.size() - 1; i >= 0; --i) {
        GtkTreeIter iter;
        gtk_list_store_append(bookmark_store, &iter);
        gtk_list_store_set(bookmark_store, &iter, REC_TEXT, bookmark_list[i].title.c_str(),
            REC_URL, bookmark_list[i].url.c_str(), REC_TIME, bookmark_list[i].time.c_str(),
            REC_SELECTED, FALSE, REC_INDEX, i, -1);
    }
}

void refresh_log_ui() {
    if (!log_store) return;
    gtk_list_store_clear(log_store);
    for (int i = (int)log_lines.size() - 1; i >= 0; --i) {
        GtkTreeIter iter;
        gtk_list_store_append(log_store, &iter);
        gtk_list_store_set(log_store, &iter, 0, log_lines[i].c_str(), 1, FALSE, 2, i, -1);
    }
}

int record_selected_column(GtkTreeModel* model) {
    const int count = gtk_tree_model_get_n_columns(model);
    if (count == 3) return 1; // 日志
    if (count == 4) return 2; // Cookies
    return REC_SELECTED;      // 历史记录与收藏
}

void on_record_toggle(GtkCellRendererToggle* renderer, gchar* path_str, gpointer data) {
    GtkTreeModel* model = GTK_TREE_MODEL(data);
    GtkTreePath* path = gtk_tree_path_new_from_string(path_str);
    GtkTreeIter iter;
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        gboolean selected = FALSE;
        const int selected_col = record_selected_column(model);
        gtk_tree_model_get(model, &iter, selected_col, &selected, -1);
        gtk_list_store_set(GTK_LIST_STORE(model), &iter, selected_col, !selected, -1);
    }
    gtk_tree_path_free(path);
}

// 历史、收藏和日志使用独立的勾选列；访问网页不由此列表提供。
GtkWidget* create_record_tree(GtkListStore* store, bool log_mode) {
    GtkWidget* tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

    // 历史、收藏、日志均保留原版勾选列；不提供单独的“访问”操作。
    {
        GtkCellRenderer* toggle = gtk_cell_renderer_toggle_new();
        const int selected_col = log_mode ? 1 : REC_SELECTED;
        GtkTreeViewColumn* select_col = gtk_tree_view_column_new_with_attributes(
            tr("选择", "Select"), toggle, "active", selected_col, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(tree), select_col);
        record_selection_columns.push_back(select_col);
        g_signal_connect(toggle, "toggled", G_CALLBACK(on_record_toggle), store);
    }

    GtkCellRenderer* text = gtk_cell_renderer_text_new();
    g_object_set(text, "ellipsize", PANGO_ELLIPSIZE_END, "ellipsize-set", TRUE,
                 "width-chars", 20, "max-width-chars", 34, NULL);
    GtkTreeViewColumn* col = gtk_tree_view_column_new_with_attributes(
        log_mode ? tr("日志内容", "Log entry") : tr("网页标题", "Page title"),
        text, "text", 0, NULL);
    gtk_tree_view_column_set_expand(col, TRUE);
    gtk_tree_view_column_set_resizable(col, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree), col);

    if (!log_mode) {
        text = gtk_cell_renderer_text_new();
        g_object_set(text, "ellipsize", PANGO_ELLIPSIZE_END, "ellipsize-set", TRUE,
                     "width-chars", 22, "max-width-chars", 38, NULL);
        col = gtk_tree_view_column_new_with_attributes(tr("网址", "Address"), text, "text", REC_URL, NULL);
        gtk_tree_view_column_set_expand(col, TRUE);
        gtk_tree_view_column_set_resizable(col, TRUE);
        gtk_tree_view_append_column(GTK_TREE_VIEW(tree), col);
        text = gtk_cell_renderer_text_new();
        g_object_set(text, "ellipsize", PANGO_ELLIPSIZE_END, "ellipsize-set", TRUE,
                     "width-chars", 17, "max-width-chars", 22, NULL);
        col = gtk_tree_view_column_new_with_attributes(tr("时间", "Time"), text, "text", REC_TIME, NULL);
        gtk_tree_view_column_set_resizable(col, TRUE);
        gtk_tree_view_append_column(GTK_TREE_VIEW(tree), col);
    }
    return tree;
}

void set_record_multi_mode(bool enabled) {
    record_multi_mode = enabled;
    if (record_multi_button) gtk_button_set_label(GTK_BUTTON(record_multi_button), enabled ? tr("退出多选", "Exit selection") : tr("多选", "Select"));
    if (record_select_all_button) gtk_widget_set_visible(record_select_all_button, enabled);
    if (record_invert_button) gtk_widget_set_visible(record_invert_button, enabled);
    if (record_delete_button) {
        gtk_widget_set_visible(record_delete_button, enabled);
        const int active_page = record_notebook ? gtk_notebook_get_current_page(GTK_NOTEBOOK(record_notebook)) : -1;
        gtk_widget_set_sensitive(record_delete_button, active_page != 2);
    }
    if (record_copy_button) gtk_widget_set_visible(record_copy_button, enabled);
    for (GtkTreeViewColumn* column : record_selection_columns)
        gtk_tree_view_column_set_visible(column, enabled);
}

GtkListStore* current_record_store() {
    int page = gtk_notebook_get_current_page(GTK_NOTEBOOK(record_notebook));
    if (page == 0) return history_store;
    if (page == 1) return bookmark_store;
    if (page == 2) return cookie_store;
    if (page == 3) return log_store;
    return nullptr;
}

void on_record_multi(GtkWidget*, gpointer) { set_record_multi_mode(!record_multi_mode); }

void on_record_select_all(GtkWidget*, gpointer) {
    GtkListStore* store = current_record_store();
    if (!store) return;
    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter);
    while (valid) {
        const int selected_col = record_selected_column(GTK_TREE_MODEL(store));
        gtk_list_store_set(store, &iter, selected_col, TRUE, -1);
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(store), &iter);
    }
}

void on_record_invert(GtkWidget*, gpointer) {
    GtkListStore* store = current_record_store();
    if (!store) return;
    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter);
    while (valid) {
        gboolean selected = FALSE;
        const int selected_col = record_selected_column(GTK_TREE_MODEL(store));
        gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, selected_col, &selected, -1);
        gtk_list_store_set(store, &iter, selected_col, !selected, -1);
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(store), &iter);
    }
}

void on_record_copy_selected(GtkWidget*, gpointer) {
    int page = gtk_notebook_get_current_page(GTK_NOTEBOOK(record_notebook));
    GtkListStore* store = current_record_store();
    if (!store) return;
    std::string copied;
    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter);
    while (valid) {
        gboolean selected = FALSE;
        gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, record_selected_column(GTK_TREE_MODEL(store)), &selected, -1);
        if (selected) {
            gchar *a = nullptr, *b = nullptr, *c = nullptr;
            if (page == 0 || page == 1) {
                gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, REC_TEXT, &a, REC_URL, &b, REC_TIME, &c, -1);
                copied += (a ? a : "") + std::string("\t") + (b ? b : "") + "\t" + (c ? c : "") + "\n";
            } else if (page == 2) {
                gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 0, &a, 1, &b, -1);
                copied += std::string(a ? a : "") + "\t" + (b ? b : "") + "\n";
            } else {
                gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 0, &a, -1);
                copied += std::string(a ? a : "") + "\n";
            }
            g_free(a); g_free(b); g_free(c);
        }
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(store), &iter);
    }
    if (copied.empty()) {
        update_status_text(tr("请先勾选要复制的项目", "Select items to copy first"), TRUE);
        return;
    }
    GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_set_text(clipboard, copied.c_str(), (gint)copied.size());
    update_status_text(tr("已复制选中内容", "Selected items copied"), TRUE);
}

void on_record_delete_selected(GtkWidget*, gpointer) {
    int page = gtk_notebook_get_current_page(GTK_NOTEBOOK(record_notebook));
    GtkListStore* store = current_record_store();
    if (!store) return;
    if (page == 2) {
        update_status_text(tr("Cookie 目前支持复制选中内容，删除请使用清空", "Cookies can be copied selectively; use Clear to remove them"), TRUE);
        return;
    }
    std::vector<int> indices;
    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter);
    while (valid) {
        gboolean selected = FALSE;
        gint index = -1;
        const bool log_mode = page == 3;
        gtk_tree_model_get(GTK_TREE_MODEL(store), &iter,
            log_mode ? 1 : REC_SELECTED, &selected,
            log_mode ? 2 : REC_INDEX, &index, -1);
        if (selected && index >= 0) indices.push_back(index);
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(store), &iter);
    }
    std::sort(indices.rbegin(), indices.rend());
    for (int index : indices) {
        if (page == 0 && index < (int)history_list.size()) history_list.erase(history_list.begin() + index);
        else if (page == 1 && index < (int)bookmark_list.size()) bookmark_list.erase(bookmark_list.begin() + index);
        else if (page == 3 && index < (int)log_lines.size()) log_lines.erase(log_lines.begin() + index);
    }
    if (page == 0) { write_records(history_file, history_list); refresh_history_ui(); }
    else if (page == 1) { write_records(bookmark_file, bookmark_list); refresh_bookmark_ui(); }
    else if (page == 3) {
        FILE* f = fopen(log_file.c_str(), "w");
        if (f) { for (const auto& line : log_lines) fprintf(f, "%s\n", line.c_str()); fclose(f); }
        refresh_log_ui();
    }
}

void on_cookies_fetched(GObject* source_object, GAsyncResult* res, gpointer user_data) {
    GError* error = nullptr;
    GList* cookies = webkit_cookie_manager_get_all_cookies_finish(WEBKIT_COOKIE_MANAGER(source_object), res, &error);
    gtk_list_store_clear(cookie_store);
    if (cookies) {
        for (GList* l = cookies; l != nullptr; l = l->next) {
            SoupCookie* cookie = (SoupCookie*)l->data;
            const gchar* domain = soup_cookie_get_domain(cookie);
            const gchar* name = soup_cookie_get_name(cookie);
            GtkTreeIter iter;
            gtk_list_store_append(cookie_store, &iter);
            gtk_list_store_set(cookie_store, &iter, 0, domain ? domain : "", 1, name ? name : "", 2, FALSE, 3, gtk_tree_model_iter_n_children(GTK_TREE_MODEL(cookie_store), NULL), -1);
        }
        g_list_free_full(cookies, (GDestroyNotify)soup_cookie_free);
    }
    if (error) g_error_free(error);
}

void on_history(GtkWidget* w, gpointer d) {
    read_records(history_file, history_list);
    read_records(bookmark_file, bookmark_list);
    record_multi_mode = false;
    record_selection_columns.clear();

    record_dialog = gtk_dialog_new_with_buttons("Browsing Data",
        GTK_WINDOW(gtk_widget_get_toplevel(w)),
        (GtkDialogFlags)(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
        tr("关闭", "Close"), GTK_RESPONSE_CLOSE, NULL);
    gtk_window_set_default_size(GTK_WINDOW(record_dialog), 680, 420);
    GtkWidget* content = gtk_dialog_get_content_area(GTK_DIALOG(record_dialog));
    record_notebook = gtk_notebook_new();
    gtk_box_pack_start(GTK_BOX(content), record_notebook, TRUE, TRUE, 0);

    GtkWidget* scroll1 = gtk_scrolled_window_new(NULL, NULL);
    history_store = gtk_list_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_BOOLEAN, G_TYPE_INT);
    GtkWidget* tree1 = create_record_tree(history_store, false);
    gtk_container_add(GTK_CONTAINER(scroll1), tree1);
    gtk_notebook_append_page(GTK_NOTEBOOK(record_notebook), scroll1, gtk_label_new(tr("访问记录", "History")));
    refresh_history_ui();

    GtkWidget* scroll2 = gtk_scrolled_window_new(NULL, NULL);
    bookmark_store = gtk_list_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_BOOLEAN, G_TYPE_INT);
    GtkWidget* tree2 = create_record_tree(bookmark_store, false);
    gtk_container_add(GTK_CONTAINER(scroll2), tree2);
    gtk_notebook_append_page(GTK_NOTEBOOK(record_notebook), scroll2, gtk_label_new(tr("网页收藏", "Bookmarks")));
    refresh_bookmark_ui();

    GtkWidget* scroll3 = gtk_scrolled_window_new(NULL, NULL);
    cookie_store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_BOOLEAN, G_TYPE_INT);
    GtkWidget* tree3 = gtk_tree_view_new_with_model(GTK_TREE_MODEL(cookie_store));
    GtkCellRenderer* cookie_toggle = gtk_cell_renderer_toggle_new();
    GtkTreeViewColumn* cookie_select_col = gtk_tree_view_column_new_with_attributes(tr("选择", "Select"), cookie_toggle, "active", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree3), cookie_select_col);
    record_selection_columns.push_back(cookie_select_col);
    g_signal_connect(cookie_toggle, "toggled", G_CALLBACK(on_record_toggle), cookie_store);
    GtkCellRenderer* cr = gtk_cell_renderer_text_new();
    g_object_set(cr, "ellipsize", PANGO_ELLIPSIZE_END, "ellipsize-set", TRUE, "width-chars", 22, "max-width-chars", 36, NULL);
    GtkTreeViewColumn* cookie_domain_col = gtk_tree_view_column_new_with_attributes(tr("域名", "Domain"), cr, "text", 0, NULL);
    gtk_tree_view_column_set_expand(cookie_domain_col, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree3), cookie_domain_col);
    cr = gtk_cell_renderer_text_new();
    g_object_set(cr, "ellipsize", PANGO_ELLIPSIZE_END, "ellipsize-set", TRUE, "width-chars", 18, "max-width-chars", 30, NULL);
    GtkTreeViewColumn* cookie_name_col = gtk_tree_view_column_new_with_attributes(tr("名称", "Name"), cr, "text", 1, NULL);
    gtk_tree_view_column_set_expand(cookie_name_col, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree3), cookie_name_col);
    gtk_container_add(GTK_CONTAINER(scroll3), tree3);
    gtk_notebook_append_page(GTK_NOTEBOOK(record_notebook), scroll3, gtk_label_new("Cookies"));
    webkit_cookie_manager_get_all_cookies(cookie_manager, NULL, on_cookies_fetched, NULL);

    GtkWidget* scroll4 = gtk_scrolled_window_new(NULL, NULL);
    log_store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_BOOLEAN, G_TYPE_INT);
    GtkWidget* log_tree = create_record_tree(log_store, true);
    gtk_container_add(GTK_CONTAINER(scroll4), log_tree);
    gtk_notebook_append_page(GTK_NOTEBOOK(record_notebook), scroll4, gtk_label_new(tr("日志", "Logs")));
    log_lines.clear();
    FILE* lf = fopen(log_file.c_str(), "r");
    if (lf) {
        char buf[4096];
        while (fgets(buf, sizeof(buf), lf)) {
            std::string line(buf);
            while (!line.empty() && (line.back() == '\n' || line.back() == '\r')) line.pop_back();
            log_lines.push_back(line);
        }
        fclose(lf);
    }
    refresh_log_ui();

    GtkWidget* fixed_toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_container_set_border_width(GTK_CONTAINER(fixed_toolbar), 8);
    record_multi_button = gtk_button_new_with_label(tr("多选", "Select"));
    record_select_all_button = gtk_button_new_with_label(tr("全选", "Select all"));
    record_invert_button = gtk_button_new_with_label(tr("反选", "Invert"));
    record_delete_button = gtk_button_new_with_label(tr("删除选中", "Delete selected"));
    record_copy_button = gtk_button_new_with_label(tr("复制选中", "Copy selected"));
    gtk_box_pack_start(GTK_BOX(fixed_toolbar), record_multi_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(fixed_toolbar), record_select_all_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(fixed_toolbar), record_invert_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(fixed_toolbar), record_delete_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(fixed_toolbar), record_copy_button, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(content), fixed_toolbar, FALSE, FALSE, 0);
    g_signal_connect(record_multi_button, "clicked", G_CALLBACK(on_record_multi), NULL);
    g_signal_connect(record_select_all_button, "clicked", G_CALLBACK(on_record_select_all), NULL);
    g_signal_connect(record_invert_button, "clicked", G_CALLBACK(on_record_invert), NULL);
    g_signal_connect(record_delete_button, "clicked", G_CALLBACK(on_record_delete_selected), NULL);
    g_signal_connect(record_copy_button, "clicked", G_CALLBACK(on_record_copy_selected), NULL);
    g_signal_connect(record_notebook, "switch-page", G_CALLBACK(+[](GtkNotebook*, GtkWidget*, guint page, gpointer) {
        set_record_multi_mode(false);
        const gboolean supports_selection = (page >= 0 && page <= 3);
        if (record_multi_button) gtk_widget_set_visible(record_multi_button, supports_selection);
        if (record_select_all_button) gtk_widget_set_visible(record_select_all_button, supports_selection && record_multi_mode);
        if (record_invert_button) gtk_widget_set_visible(record_invert_button, supports_selection && record_multi_mode);
        if (record_delete_button) gtk_widget_set_visible(record_delete_button, supports_selection && record_multi_mode);
        if (record_copy_button) gtk_widget_set_visible(record_copy_button, supports_selection && record_multi_mode);
    }), NULL);
    set_record_multi_mode(false);
    gtk_widget_hide(record_multi_button);
    gtk_widget_hide(record_select_all_button);
    gtk_widget_hide(record_invert_button);
    gtk_widget_hide(record_delete_button);
    gtk_widget_hide(record_copy_button);

    gtk_widget_show_all(record_dialog);
    set_record_multi_mode(false);
    gtk_dialog_run(GTK_DIALOG(record_dialog));
    gtk_widget_destroy(record_dialog);
    record_dialog = nullptr;
    record_notebook = nullptr;
    record_selection_columns.clear();
    history_store = bookmark_store = cookie_store = log_store = nullptr;
    record_multi_button = record_select_all_button = record_invert_button = record_delete_button = record_copy_button = nullptr;
}

void on_bookmark(GtkWidget* w, gpointer d) {
    GtkWidget* webview = get_current_webview();
    if (!webview) return;
    const gchar* uri = webkit_web_view_get_uri(WEBKIT_WEB_VIEW(webview));
    const gchar* title = webkit_web_view_get_title(WEBKIT_WEB_VIEW(webview));
    
    if (uri) {
        std::string t = (title && strlen(title) > 0) ? title : uri;
        for (char& c : t) { if (c == '|' || c == '\n' || c == '\r') c = ' '; }
        time_t now = time(0);
        char time_buf[64];
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        
        for (const auto& item : bookmark_list) {
            if (item.url == uri) {
                GtkWidget* dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(w)), (GtkDialogFlags)GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "该页面已收藏！");
                gtk_dialog_run(GTK_DIALOG(dialog));
                gtk_widget_destroy(dialog);
                return;
            }
        }
        bookmark_list.push_back({time_buf, t, uri});
        write_records(bookmark_file, bookmark_list);
        GtkWidget* dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(w)), (GtkDialogFlags)GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "收藏成功！");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

void apply_page_zoom(GtkWidget* view);

gboolean on_url_entry_focus_in(GtkWidget* widget, GdkEventFocus* event, gpointer user_data) {
    is_typing = TRUE;
    return FALSE;
}

gboolean on_url_entry_focus_out(GtkWidget* widget, GdkEventFocus* event, gpointer user_data) {
    is_typing = FALSE;
    GtkWidget* webview = get_current_webview();
    if (webview) {
        const gchar* uri = webkit_web_view_get_uri(WEBKIT_WEB_VIEW(webview));
        gtk_entry_set_text(GTK_ENTRY(url_entry), uri ? uri : "");
        apply_page_zoom(webview);
    }
    return FALSE;
}

gboolean hide_progress_if_idle(gpointer) {
    GtkWidget* current = get_current_webview();
    if (progress_bar && (!current || !webkit_web_view_is_loading(WEBKIT_WEB_VIEW(current))))
        gtk_widget_hide(progress_bar);
    return G_SOURCE_REMOVE;
}

void on_load_changed(WebKitWebView* web_view, WebKitLoadEvent load_event, gpointer user_data) {
    gboolean is_foreground = (GTK_WIDGET(web_view) == get_current_webview());

    if (load_event == WEBKIT_LOAD_STARTED) {
        if (is_foreground) {
            update_status_text(tr("正在连接...", "Connecting..."), FALSE);
            gtk_widget_show(progress_bar);
            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 0.0);
        }
    } else if (load_event == WEBKIT_LOAD_REDIRECTED) {
        if (is_foreground) update_status_text(tr("正在重定向...", "Redirecting..."), FALSE);
    } else if (load_event == WEBKIT_LOAD_COMMITTED) {
        if (is_foreground) update_status_text(tr("等待响应...", "Waiting for response..."), FALSE);
    } else if (load_event == WEBKIT_LOAD_FINISHED) {
        if (is_foreground) {
            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 1.0);
            g_timeout_add(450, hide_progress_if_idle, NULL);
            update_status_text(tr("完成", "Done"), TRUE);
        }
        
        const gchar* uri = webkit_web_view_get_uri(web_view);
        const gchar* title = webkit_web_view_get_title(web_view);
        save_history(uri, title);
        write_log("加载完成: " + std::string(uri ? uri : "未知"));
        
        if (is_foreground && !is_typing) {
            gtk_entry_set_text(GTK_ENTRY(url_entry), uri ? uri : "");
        }
    }
}


std::string html_escape(const char* value) {
    gchar* escaped = g_markup_escape_text(value ? value : "", -1);
    std::string result = escaped ? escaped : "";
    g_free(escaped);
    return result;
}

std::string error_reason(GError* error) {
    if (!error) return tr("未知网络错误，请稍后重试。", "An unknown network error occurred. Please try again.");
    if (error->domain == WEBKIT_NETWORK_ERROR) {
        switch (error->code) {
            case WEBKIT_NETWORK_ERROR_FAILED: return tr("无法连接到服务器。请检查网络连接、代理或服务器状态后重试。", "Could not connect to the server. Check your connection, proxy, or server status and try again.");
            case WEBKIT_NETWORK_ERROR_TRANSPORT: return tr("与服务器通信时连接中断。请检查网络后重试。", "The connection was interrupted while communicating with the server. Check your network and try again.");
            case WEBKIT_NETWORK_ERROR_FILE_DOES_NOT_EXIST: return tr("服务器上的请求资源不存在，请检查网址是否正确。", "The requested resource does not exist on the server. Check the URL.");
            case WEBKIT_NETWORK_ERROR_UNKNOWN_PROTOCOL: return tr("浏览器无法识别该网址协议。请检查网址是否以 http:// 或 https:// 开头。", "The browser does not recognize this URL scheme. Check that it uses http:// or https://.");
            default: break;
        }
    }
    if (error->domain == WEBKIT_POLICY_ERROR)
        return tr("浏览器策略阻止了此次访问，可能是网址、证书或页面权限问题。", "The browser policy blocked this navigation. The URL, certificate, or page permissions may be responsible.");
    if (error->domain == G_IO_ERROR && error->code == G_IO_ERROR_TIMED_OUT)
        return tr("网页加载超时。服务器没有在规定时间内响应，请稍后重试。", "The page load timed out. The server did not respond in time. Please try again later.");
    std::string message = error->message ? error->message : "";
    std::string lower = message;
    for (char& c : lower) c = (char)g_ascii_tolower(c);
    if (lower.find("resolve") != std::string::npos || lower.find("dns") != std::string::npos || lower.find("name or service not known") != std::string::npos)
        return tr("无法解析服务器地址。请检查网址拼写、DNS 或网络连接。", "The server address could not be resolved. Check the URL, DNS, or network connection.");
    if (lower.find("refused") != std::string::npos || message.find("拒绝") != std::string::npos)
        return tr("服务器拒绝了连接请求。服务器可能暂时不可用，或没有开放对应端口。", "The server refused the connection. It may be unavailable or not accepting connections on that port.");
    if (lower.find("timed out") != std::string::npos || lower.find("timeout") != std::string::npos)
        return tr("网页加载超时。请检查网络连接，或稍后重新加载。", "The page load timed out. Check your connection or reload later.");
    if (lower.find("certificate") != std::string::npos || lower.find("tls") != std::string::npos || lower.find("ssl") != std::string::npos)
        return tr("安全连接建立失败，可能是网站证书无效或设备时间不正确。", "A secure connection could not be established. The site certificate may be invalid or the device clock may be incorrect.");
    return tr("网页无法完成加载。请检查网络连接、网址是否正确，或稍后重试。", "The page could not be loaded. Check your network connection and URL, or try again later.");
}

std::string error_code_name(GError* error) {
    if (!error) return "UNKNOWN";
    if (error->domain == WEBKIT_NETWORK_ERROR) {
        switch (error->code) {
            case WEBKIT_NETWORK_ERROR_FAILED: return "WEBKIT_NETWORK_ERROR_FAILED";
            case WEBKIT_NETWORK_ERROR_TRANSPORT: return "WEBKIT_NETWORK_ERROR_TRANSPORT";
            case WEBKIT_NETWORK_ERROR_FILE_DOES_NOT_EXIST: return "WEBKIT_NETWORK_ERROR_FILE_DOES_NOT_EXIST";
            case WEBKIT_NETWORK_ERROR_UNKNOWN_PROTOCOL: return "WEBKIT_NETWORK_ERROR_UNKNOWN_PROTOCOL";
            case WEBKIT_NETWORK_ERROR_CANCELLED: return "WEBKIT_NETWORK_ERROR_CANCELLED";
            default: return "WEBKIT_NETWORK_ERROR";
        }
    }
    if (error->domain == WEBKIT_POLICY_ERROR) return "WEBKIT_POLICY_ERROR";
    if (error->domain == G_IO_ERROR) return "G_IO_ERROR";
    return g_quark_to_string(error->domain) ? g_quark_to_string(error->domain) : "UNKNOWN_ERROR_DOMAIN";
}

gboolean on_load_failed(WebKitWebView* web_view, WebKitLoadEvent load_event, const gchar* failing_uri, GError* error, gpointer user_data) {
    if (error && g_error_matches(error, WEBKIT_PLUGIN_ERROR, WEBKIT_PLUGIN_ERROR_WILL_HANDLE_LOAD)) return TRUE;
    if (error && g_error_matches(error, WEBKIT_PLUGIN_ERROR, WEBKIT_PLUGIN_ERROR_CANNOT_FIND_PLUGIN)) {
        write_log("媒体插件缺失: " + std::string(error->message));
        return TRUE;
    }
    if (error && (g_error_matches(error, WEBKIT_NETWORK_ERROR, WEBKIT_NETWORK_ERROR_CANCELLED) ||
                  g_error_matches(error, G_IO_ERROR, G_IO_ERROR_CANCELLED))) return TRUE;
    if (error && error->message && (strstr(error->message, "cancelled") || strstr(error->message, "Cancelled"))) return TRUE;

    const gchar* uri = failing_uri ? failing_uri : "";
    const std::string reason = error_reason(error);
    const std::string escaped_reason = html_escape(reason.c_str());
    const std::string escaped_uri = html_escape(uri);
    const std::string escaped_detail = html_escape(error && error->message ? error->message : "Unknown error");
    const std::string code_text = error_code_name(error) + (error ? " (" + std::to_string(error->code) + ")" : "");
    const std::string escaped_code = html_escape(code_text.c_str());
    const bool dark = ui_theme == "dark";
    const std::string bg = dark ? "#202630" : "#f5f7fb";
    const std::string fg = dark ? "#edf0f5" : "#20242b";
    const std::string muted = dark ? "#c4cbd6" : "#596273";
    const std::string panel = dark ? "#303946" : "#e9edf4";
    std::string html =
        "<!doctype html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'>"
        "<title>" + std::string(tr("网页无法访问", "This site can’t be reached")) + "</title><style>"
        "*{box-sizing:border-box}html,body{min-height:100%;margin:0;font-family:Arial,'Noto Sans CJK SC',sans-serif;background:" + bg + ";color:" + fg + ";}"
        "body{padding:clamp(18px,5vw,42px)}.wrap{width:100%;max-width:780px;margin:clamp(12px,7vh,64px) auto 32px;}"
        ".x{color:#e53935;font-size:64px;font-weight:700;line-height:1;margin:0 0 18px 0;}"
        "h1{font-size:clamp(24px,4vw,32px);line-height:1.25;margin:0 0 14px;font-weight:650;}"
        "p{font-size:16px;line-height:1.75;color:" + muted + ";margin:0 0 14px;overflow-wrap:anywhere;word-break:break-word;}"
        ".url{display:block;margin:18px 0;padding:12px 14px;border-radius:9px;background:" + panel + ";overflow-wrap:anywhere;word-break:break-all;}"
        "a{color:#4285f4;text-decoration:none}a:hover{text-decoration:underline}"
        ".details{margin-top:22px;padding:16px;border-radius:10px;background:" + panel + ";overflow-wrap:anywhere;word-break:break-word;}"
        ".detail-title{font-weight:600;margin-bottom:8px;color:" + fg + ";}.code{font-family:monospace;font-size:13px;line-height:1.6;}"
        ".raw{margin-top:8px;color:" + muted + ";white-space:pre-wrap;}"
        "@media(max-width:480px){body{padding:18px}.wrap{margin:18px auto}.x{font-size:54px}p{font-size:15px}}"
        "</style></head><body><main class='wrap'>"
        "<div class='x'>×</div><h1>" + std::string(tr("网页无法访问", "This site can’t be reached")) + "</h1>"
        "<p>" + escaped_reason + "</p>"
        "<div class='url'><a target='_blank' rel='noopener' href='" + escaped_uri + "'>" + escaped_uri + "</a></div>"
        "<section class='details'><div class='detail-title'>" + std::string(tr("错误详情", "Error details")) + "</div>"
        "<div class='code'>" + std::string(tr("错误码：", "Error code: ")) + escaped_code + "</div>"
        "<div class='raw'>" + escaped_detail + "</div></section></main></body></html>";

    write_log("加载失败: " + std::string(uri ? uri : "未知网址") + " 错误: " + (error && error->message ? error->message : "unknown"));
    if (GTK_WIDGET(web_view) == get_current_webview()) {
        gtk_widget_hide(progress_bar);
        update_status_text(tr("网页无法访问", "Page unavailable"), FALSE);
    }
    webkit_web_view_load_alternate_html(web_view, html.c_str(), uri, NULL);
    return TRUE;
}

void on_web_process_terminated(WebKitWebView* web_view, WebKitWebProcessTerminationReason reason, gpointer user_data) {
    const gchar* uri = webkit_web_view_get_uri(web_view);
    if (uri) webkit_web_view_load_uri(web_view, uri);
}

void on_url_activate(GtkWidget* widget, gpointer data) {
    const char* input = gtk_entry_get_text(GTK_ENTRY(url_entry));
    std::string target = resolve_input(input);
    write_log("用户输入导航: " + target);
    is_typing = FALSE; 
    GtkWidget* webview = get_current_webview();
    if (webview) webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webview), target.c_str());
}

// Preserve the original adaptive zoom formula: viewport width / 1366, clamped to 0.3..1.0.
// The user preference multiplies that original ratio; 100% is the exact original behavior.
void apply_page_zoom(GtkWidget* view) {
    if (!view || !WEBKIT_IS_WEB_VIEW(view)) return;
    const gint width = gtk_widget_get_allocated_width(view);
    if (width <= 0) return;
    double zoom = (double)width / 1366.0;
    if (zoom < 0.3) zoom = 0.3;
    if (zoom > 1.0) zoom = 1.0;
    zoom *= page_zoom_percent / 100.0;
    if (zoom < 0.15) zoom = 0.15;
    if (zoom > 1.0) zoom = 1.0;
    const double current = webkit_web_view_get_zoom_level(WEBKIT_WEB_VIEW(view));
    if (std::fabs(current - zoom) > 0.01)
        webkit_web_view_set_zoom_level(WEBKIT_WEB_VIEW(view), zoom);
}

gboolean delayed_webview_zoom(gpointer) {
    resize_timer_id = 0;
    GtkWidget* view = get_current_webview();
    if (view) apply_page_zoom(view);
    return G_SOURCE_REMOVE;
}

void on_window_size_allocate(GtkWidget*, GtkAllocation*, gpointer) {
    if (resize_timer_id > 0) {
        g_source_remove(resize_timer_id);
        resize_timer_id = 0;
    }
    resize_timer_id = g_timeout_add(150, delayed_webview_zoom, NULL);
}

void apply_zoom_to_all_tabs() {
    if (!notebook) return;
    for (int i = 0; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)); ++i) {
        GtkWidget* page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
        GtkWidget* view = get_webview_from_page(page);
        if (view) apply_page_zoom(view);
    }
}

void add_tab(const char* url) {
    GtkWidget* webview = webkit_web_view_new_with_context(web_context);
    gtk_widget_set_hexpand(webview, TRUE);
    gtk_widget_set_vexpand(webview, TRUE);
    
    WebKitSettings* settings = webkit_web_view_get_settings(WEBKIT_WEB_VIEW(webview));
    webkit_settings_set_media_playback_requires_user_gesture(settings, media_requires_gesture);
    webkit_settings_set_enable_javascript(settings, javascript_enabled);
    webkit_settings_set_enable_webgl(settings, webgl_enabled);
    webkit_settings_set_enable_write_console_messages_to_stdout(settings, FALSE);

    g_signal_connect(webview, "load-changed", G_CALLBACK(on_load_changed), NULL);
    g_signal_connect(webview, "load-failed", G_CALLBACK(on_load_failed), NULL);
    g_signal_connect(webview, "web-process-terminated", G_CALLBACK(on_web_process_terminated), NULL);
    g_signal_connect(webview, "notify::estimated-load-progress", G_CALLBACK(on_progress_changed), NULL);
    g_signal_connect(webview, "decide-policy", G_CALLBACK(on_decide_policy), NULL);
    
    
    g_signal_connect(webview, "notify::title", G_CALLBACK(+[](GtkWidget* w, GParamSpec*, gpointer){
        const gchar* title = webkit_web_view_get_title(WEBKIT_WEB_VIEW(w));
        GtkWidget* page_widget = gtk_widget_get_parent(w);
        gint page = page_widget ? gtk_notebook_page_num(GTK_NOTEBOOK(notebook), page_widget) : -1;
        if (page >= 0) gtk_notebook_set_tab_label_text(GTK_NOTEBOOK(notebook), page_widget,
            (title && *title) ? title : tr("新标签页", "New tab"));
    }), NULL);

    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(vbox), webview, TRUE, TRUE, 0);
    gtk_widget_show_all(vbox);

    int page = gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, gtk_label_new(tr("新标签页", "New tab")));
    gtk_widget_show_all(vbox);
    gtk_widget_show_all(gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), page));
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), page);
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webview), url);
}

void on_switch_page(GtkNotebook* nb, GtkWidget* page, guint page_num, gpointer data) {
    is_typing = FALSE; 
    GtkWidget* webview = get_webview_from_page(page);
    if (webview) {
        const gchar* uri = webkit_web_view_get_uri(WEBKIT_WEB_VIEW(webview));
        gtk_entry_set_text(GTK_ENTRY(url_entry), uri ? uri : "");
        apply_page_zoom(webview);

        if (webkit_web_view_is_loading(WEBKIT_WEB_VIEW(webview))) {
            gtk_widget_show(progress_bar);
            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 
                webkit_web_view_get_estimated_load_progress(WEBKIT_WEB_VIEW(webview)));
            update_status_text(tr("加载中...", "Loading..."), FALSE);
        } else {
            gtk_widget_hide(progress_bar);
            update_status_text(tr("完成", "Done"), TRUE);
        }
    }
}

void on_back(GtkWidget* w, gpointer d) { GtkWidget* wv = get_current_webview(); if (wv) webkit_web_view_go_back(WEBKIT_WEB_VIEW(wv)); }
void on_forward(GtkWidget* w, gpointer d) { GtkWidget* wv = get_current_webview(); if (wv) webkit_web_view_go_forward(WEBKIT_WEB_VIEW(wv)); }
void on_refresh(GtkWidget* w, gpointer d) { GtkWidget* wv = get_current_webview(); if (wv) webkit_web_view_reload(WEBKIT_WEB_VIEW(wv)); }
void on_new_tab(GtkWidget* w, gpointer d) { add_tab("https://cn.bing.com"); }

void on_close_tab(GtkWidget* w, gpointer d) {
    int page_idx = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
    if (page_idx == -1) return;

    gtk_notebook_remove_page(GTK_NOTEBOOK(notebook), page_idx);
    if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) == 0) {
        add_tab("https://cn.bing.com");
    }
}


void show_readonly_text_dialog(GtkWidget* parent, const char* title, const char* body) {
    GtkWidget* dialog = gtk_dialog_new_with_buttons(title ? title : "Information", GTK_WINDOW(parent),
        (GtkDialogFlags)(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
        tr("关闭", "Close"), GTK_RESPONSE_CLOSE, NULL);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 620, 520);
    GtkWidget* area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget* scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    GtkWidget* view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD_CHAR);
    gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(view)), body ? body : "", -1);
    gtk_container_add(GTK_CONTAINER(scroll), view);
    gtk_box_pack_start(GTK_BOX(area), scroll, TRUE, TRUE, 8);
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void show_project_info(GtkWidget* parent) {
    GtkWidget* dialog = gtk_dialog_new_with_buttons("About WebBrowser", GTK_WINDOW(parent),
        (GtkDialogFlags)(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
        tr("关闭", "Close"), GTK_RESPONSE_CLOSE, NULL);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 560, 420);
    GtkWidget* area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget* scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    GtkWidget* label = gtk_label_new(NULL);
    gtk_label_set_xalign(GTK_LABEL(label), 0.0f);
    gtk_label_set_yalign(GTK_LABEL(label), 0.0f);
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
    gtk_label_set_selectable(GTK_LABEL(label), TRUE);
    const char* markup =
        "<b>WebBrowser</b>\n\n"
        "由 Sakuragi Group 开发的轻量级 Web 浏览器，面向 Termux:X11 环境。\n\n"
        "A lightweight web browser developed by Sakuragi Group for the Termux:X11 environment.\n\n"
        "制作团队 / Developer: Sakuragi Group\n"
        "项目仓库 / Repository: <a href='https://github.com/YamaiSakuragi/WebBrowser'>https://github.com/YamaiSakuragi/WebBrowser</a>\n"
        "Telegram: <a href='https://t.me/YamaiSakuragi/'>https://t.me/YamaiSakuragi/</a>";
    gtk_label_set_markup(GTK_LABEL(label), markup);
    g_signal_connect(label, "activate-link", G_CALLBACK(+[](GtkLabel*, const gchar* uri, gpointer) -> gboolean {
        if (uri && *uri) add_tab(uri);
        return TRUE;
    }), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(scroll), 16);
    gtk_container_add(GTK_CONTAINER(scroll), label);
    gtk_box_pack_start(GTK_BOX(area), scroll, TRUE, TRUE, 0);
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void show_license(GtkWidget* parent) {
    show_readonly_text_dialog(parent, "GNU GPL v3 License", GPL3_TEXT);
}


void on_settings(GtkWidget* w, gpointer) {
    GtkWidget* parent = gtk_widget_get_toplevel(w);
    GtkWidget* dialog = gtk_dialog_new_with_buttons("Settings", GTK_WINDOW(parent),
        (GtkDialogFlags)(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
        tr("保存", "Save"), GTK_RESPONSE_ACCEPT,
        tr("取消", "Cancel"), GTK_RESPONSE_CANCEL, NULL);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 420, 560);
    gtk_window_set_resizable(GTK_WINDOW(dialog), TRUE);
    GtkWidget* area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget* scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_container_set_border_width(GTK_CONTAINER(box), 16);
    gtk_container_add(GTK_CONTAINER(scroll), box);
    gtk_box_pack_start(GTK_BOX(area), scroll, TRUE, TRUE, 0);

    GtkWidget* appearance_frame = gtk_frame_new(tr("外观与浏览", "Appearance and browsing"));
    GtkWidget* appearance = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(appearance), 9);
    gtk_grid_set_column_spacing(GTK_GRID(appearance), 12);
    gtk_container_set_border_width(GTK_CONTAINER(appearance), 12);
    gtk_container_add(GTK_CONTAINER(appearance_frame), appearance);

    GtkWidget* theme_label = gtk_label_new(tr("颜色模式", "Color mode"));
    gtk_widget_set_halign(theme_label, GTK_ALIGN_START);
    GtkWidget* theme_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(theme_combo), "light", tr("浅色", "Light"));
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(theme_combo), "dark", tr("深色", "Dark"));
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(theme_combo), ui_theme.c_str());
    gtk_grid_attach(GTK_GRID(appearance), theme_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(appearance), theme_combo, 1, 0, 1, 1);

    GtkWidget* lang_label = gtk_label_new(tr("界面语言", "Language"));
    gtk_widget_set_halign(lang_label, GTK_ALIGN_START);
    GtkWidget* lang_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(lang_combo), "zh", "中文");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(lang_combo), "en", "English");
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(lang_combo), ui_language.c_str());
    gtk_grid_attach(GTK_GRID(appearance), lang_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(appearance), lang_combo, 1, 1, 1, 1);

    GtkWidget* zoom_label = gtk_label_new(tr("网页缩放", "Page zoom"));
    gtk_widget_set_halign(zoom_label, GTK_ALIGN_START);
    GtkWidget* zoom_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.0, 100.0, 1.0);
    gtk_range_set_value(GTK_RANGE(zoom_scale), page_zoom_percent);
    gtk_scale_set_digits(GTK_SCALE(zoom_scale), 0);
    gtk_scale_set_value_pos(GTK_SCALE(zoom_scale), GTK_POS_RIGHT);
    gtk_scale_add_mark(GTK_SCALE(zoom_scale), 0.0, GTK_POS_BOTTOM, "0%");
    gtk_scale_add_mark(GTK_SCALE(zoom_scale), 50.0, GTK_POS_BOTTOM, "50%");
    gtk_scale_add_mark(GTK_SCALE(zoom_scale), 100.0, GTK_POS_BOTTOM, "100%");
    gtk_widget_set_hexpand(zoom_scale, TRUE);
    GtkWidget* zoom_note = gtk_label_new(tr("范围 0%–100%，默认 50%；按原版自适应比例计算。", "Range 0%–100%, default 50%; based on the original adaptive ratio."));
    gtk_label_set_line_wrap(GTK_LABEL(zoom_note), TRUE);
    gtk_widget_set_halign(zoom_note, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(appearance), zoom_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(appearance), zoom_scale, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(appearance), zoom_note, 0, 3, 2, 1);
    gtk_box_pack_start(GTK_BOX(box), appearance_frame, FALSE, FALSE, 0);

    GtkWidget* debug_frame = gtk_frame_new(tr("开发者设置", "Developer Settings"));
    GtkWidget* debug_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(debug_box), 12);
    gtk_container_add(GTK_CONTAINER(debug_frame), debug_box);
    GtkWidget* js_check = gtk_check_button_new_with_label(tr("启用 JavaScript", "Enable JavaScript"));
    GtkWidget* webgl_check = gtk_check_button_new_with_label(tr("启用 WebGL", "Enable WebGL"));
    GtkWidget* media_check = gtk_check_button_new_with_label(tr("媒体播放需要用户操作", "Require user gesture for media playback"));
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(js_check), javascript_enabled);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(webgl_check), webgl_enabled);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(media_check), media_requires_gesture);
    gtk_box_pack_start(GTK_BOX(debug_box), js_check, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(debug_box), webgl_check, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(debug_box), media_check, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), debug_frame, FALSE, FALSE, 0);

    GtkWidget* about_frame = gtk_frame_new(tr("关于", "About"));
    GtkWidget* about_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 7);
    gtk_container_set_border_width(GTK_CONTAINER(about_box), 12);
    gtk_container_add(GTK_CONTAINER(about_frame), about_box);
    GtkWidget* team = gtk_label_new(tr("制作团队：Sakuragi Group", "Developed by Sakuragi Group"));
    gtk_widget_set_halign(team, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(about_box), team, FALSE, FALSE, 0);
    GtkWidget* copyright = gtk_label_new("Copyright © Sakuragi Group");
    gtk_widget_set_halign(copyright, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(about_box), copyright, FALSE, FALSE, 0);
    GtkWidget* info_button = gtk_button_new_with_label(tr("项目说明", "Project information"));
    GtkWidget* license_button = gtk_button_new_with_label(tr("GNU GPL v3 开源协议", "GNU GPL v3 license"));
    GtkWidget* telegram_button = gtk_button_new_with_label(tr("Telegram 频道", "Telegram channel"));
    gtk_widget_set_halign(info_button, GTK_ALIGN_FILL);
    gtk_widget_set_halign(license_button, GTK_ALIGN_FILL);
    gtk_widget_set_halign(telegram_button, GTK_ALIGN_FILL);
    gtk_box_pack_start(GTK_BOX(about_box), info_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(about_box), license_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(about_box), telegram_button, FALSE, FALSE, 0);
    g_signal_connect(info_button, "clicked", G_CALLBACK(+[](GtkWidget* button, gpointer) {
        show_project_info(gtk_widget_get_toplevel(button));
    }), NULL);
    g_signal_connect(license_button, "clicked", G_CALLBACK(+[](GtkWidget* button, gpointer) {
        show_license(gtk_widget_get_toplevel(button));
    }), NULL);
    g_signal_connect(telegram_button, "clicked", G_CALLBACK(+[](GtkWidget*, gpointer) {
        add_tab("https://t.me/YamaiSakuragi/");
    }), NULL);
    gtk_box_pack_start(GTK_BOX(box), about_frame, FALSE, FALSE, 0);

    gtk_widget_show_all(dialog);
    const std::string old_language = ui_language;
    const gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_ACCEPT) {
        const gchar* selected_theme = gtk_combo_box_get_active_id(GTK_COMBO_BOX(theme_combo));
        const gchar* selected_lang = gtk_combo_box_get_active_id(GTK_COMBO_BOX(lang_combo));
        if (selected_theme) ui_theme = selected_theme;
        if (selected_lang) ui_language = selected_lang;
        page_zoom_percent = gtk_range_get_value(GTK_RANGE(zoom_scale));
        javascript_enabled = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(js_check));
        webgl_enabled = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(webgl_check));
        media_requires_gesture = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(media_check));
        save_settings();
        apply_app_theme();
        apply_ui_language();
        const bool reload_for_page_preferences = (old_language != ui_language);
        for (int i = 0; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)); ++i) {
            GtkWidget* page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
            GtkWidget* view = get_webview_from_page(page);
            if (view) {
                WebKitSettings* settings = webkit_web_view_get_settings(WEBKIT_WEB_VIEW(view));
                webkit_settings_set_enable_javascript(settings, javascript_enabled);
                webkit_settings_set_enable_webgl(settings, webgl_enabled);
                webkit_settings_set_media_playback_requires_user_gesture(settings, media_requires_gesture);
                if (reload_for_page_preferences) webkit_web_view_reload(WEBKIT_WEB_VIEW(view));
            }
        }
        apply_zoom_to_all_tabs();
    }
    gtk_widget_destroy(dialog);
}

void build_ui(GtkWidget* window) {
    mkdir("/data/data/com.termux/files/home/.config", 0777);
    mkdir("/data/data/com.termux/files/home/.config/WebBrowser", 0777);
    mkdir(base_dir.c_str(), 0777);
    mkdir((base_dir + "/history").c_str(), 0777);
    mkdir((base_dir + "/bookmarks").c_str(), 0777);
    mkdir((base_dir + "/logs").c_str(), 0777);
    mkdir((base_dir + "/cookies").c_str(), 0777);
    mkdir((base_dir + "/cache").c_str(), 0777);

    load_settings();
    apply_app_theme();
    read_records(history_file, history_list);
    read_records(bookmark_file, bookmark_list);
    write_log("浏览器启动");

    data_manager = webkit_website_data_manager_new(
        "base-data-directory", base_dir.c_str(),
        "base-cache-directory", (base_dir + "/cache").c_str(),
        NULL
    );

    webkit_website_data_manager_set_tls_errors_policy(data_manager, WEBKIT_TLS_ERRORS_POLICY_IGNORE);
    webkit_website_data_manager_set_network_proxy_settings(data_manager, WEBKIT_NETWORK_PROXY_MODE_NO_PROXY, NULL);

    cookie_manager = webkit_website_data_manager_get_cookie_manager(data_manager);

    webkit_cookie_manager_set_persistent_storage(
        cookie_manager,
        (base_dir + "/cookies/cookies.sqlite").c_str(),
        WEBKIT_COOKIE_PERSISTENT_STORAGE_SQLITE
    );

    web_context = webkit_web_context_new_with_website_data_manager(data_manager);

    const char* languages[] = {"zh-CN", "zh", "en", NULL};
    webkit_web_context_set_preferred_languages(web_context, languages);

    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    
    main_window = window;
    GtkWidget* toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(toolbar), 5);
    
    btn_back = gtk_button_new_with_label("←");
    btn_forward = gtk_button_new_with_label("→");
    btn_refresh = gtk_button_new_with_label("⟳");
    btn_history = gtk_button_new_with_label("◷");
    url_entry = gtk_entry_new();
    gtk_widget_set_hexpand(url_entry, TRUE);
    btn_bookmark = gtk_button_new_with_label("☆");
    btn_settings = gtk_button_new_with_label("⚙");
    
    g_signal_connect(url_entry, "focus-in-event", G_CALLBACK(on_url_entry_focus_in), NULL);
    g_signal_connect(url_entry, "focus-out-event", G_CALLBACK(on_url_entry_focus_out), NULL);

    gtk_box_pack_start(GTK_BOX(toolbar), btn_back, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_forward, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_refresh, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_history, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), url_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_bookmark, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_settings, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);

    progress_bar = gtk_progress_bar_new();
    gtk_widget_set_size_request(progress_bar, -1, 3);
    gtk_widget_hide(progress_bar);
    gtk_box_pack_start(GTK_BOX(vbox), progress_bar, FALSE, FALSE, 0);

    GtkWidget* overlay = gtk_overlay_new();
    gtk_box_pack_start(GTK_BOX(vbox), overlay, TRUE, TRUE, 0);

    notebook = gtk_notebook_new();
    gtk_notebook_set_scrollable(GTK_NOTEBOOK(notebook), TRUE);
    gtk_container_add(GTK_CONTAINER(overlay), notebook);

    // Place tab controls directly on the notebook tab strip instead of wasting a separate row.
    GtkWidget* tab_actions = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    tab_add_button = gtk_button_new_with_label("+");
    tab_close_button = gtk_button_new_with_label("×");
    gtk_widget_set_tooltip_text(tab_add_button, tr("新建标签页", "New tab"));
    gtk_widget_set_tooltip_text(tab_close_button, tr("关闭当前标签页", "Close current tab"));
    gtk_box_pack_start(GTK_BOX(tab_actions), tab_add_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(tab_actions), tab_close_button, FALSE, FALSE, 0);
    gtk_notebook_set_action_widget(GTK_NOTEBOOK(notebook), tab_actions, GTK_PACK_END);
    gtk_widget_show_all(tab_actions);
    g_signal_connect(tab_add_button, "clicked", G_CALLBACK(on_new_tab), NULL);
    g_signal_connect(tab_close_button, "clicked", G_CALLBACK(on_close_tab), NULL);

    status_overlay = gtk_label_new("");
    gtk_widget_set_halign(status_overlay, GTK_ALIGN_START);
    gtk_widget_set_valign(status_overlay, GTK_ALIGN_END);
    
    GtkCssProvider* css = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css, 
        "label { background-color: rgba(50, 50, 50, 0.8); color: white; padding: 4px 8px; border-radius: 4px; font-size: 12px; }", -1, NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(status_overlay), GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    gtk_widget_set_margin_start(status_overlay, 10);
    gtk_widget_set_margin_bottom(status_overlay, 10);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), status_overlay);
    gtk_widget_hide(status_overlay);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    g_signal_connect(url_entry, "activate", G_CALLBACK(on_url_activate), NULL);
    g_signal_connect(btn_back, "clicked", G_CALLBACK(on_back), NULL);
    g_signal_connect(btn_forward, "clicked", G_CALLBACK(on_forward), NULL);
    g_signal_connect(btn_refresh, "clicked", G_CALLBACK(on_refresh), NULL);
    g_signal_connect(btn_settings, "clicked", G_CALLBACK(on_settings), NULL);
    g_signal_connect(btn_history, "clicked", G_CALLBACK(on_history), NULL);
    g_signal_connect(btn_bookmark, "clicked", G_CALLBACK(on_bookmark), NULL);
    g_signal_connect(notebook, "switch-page", G_CALLBACK(on_switch_page), NULL);
    g_signal_connect(window, "size-allocate", G_CALLBACK(on_window_size_allocate), NULL);

    apply_ui_language();
    add_tab("https://cn.bing.com");

}

int main(int argc, char* argv[]) {
    g_setenv("WEBKIT_DISABLE_COMPOSITING_MODE", "1", TRUE);
    g_setenv("WEBKIT_DISABLE_DMABUF_RENDERER", "1", TRUE);
    
    // 屏蔽无障碍总线警告，防止该警告引发潜在崩溃
    g_setenv("NO_AT_BRIDGE", "1", TRUE); 
    
    // 保持 WebKit 默认合成设置，避免额外注入脚本或网页样式干扰加载。

    gtk_init(&argc, &argv);
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "WebBrowser");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 720);
    gtk_widget_set_size_request(window, 320, 240);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    build_ui(window);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}