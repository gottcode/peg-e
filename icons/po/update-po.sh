#!/bin/sh


echo -n 'Preparing files...'
cd ..

rm -f peg-e.desktop.in
cp peg-e.desktop peg-e.desktop.in
sed -e '/^Name\[/ d' \
	-e '/^GenericName\[/ d' \
	-e '/^Comment\[/ d' \
	-e 's/^Name/_Name/' \
	-e 's/^GenericName/_GenericName/' \
	-e 's/^Comment/_Comment/' \
	-i peg-e.desktop.in

rm -f peg-e.appdata.xml.in
cp peg-e.appdata.xml peg-e.appdata.xml.in
sed -e '/p xml:lang/ d' \
	-e '/summary xml:lang/ d' \
	-e '/name xml:lang/ d' \
	-e 's/<p>/<_p>/' \
	-e 's/<\/p>/<\/_p>/' \
	-e 's/<summary>/<_summary>/' \
	-e 's/<\/summary>/<\/_summary>/' \
	-e 's/<name>/<_name>/' \
	-e 's/<\/name>/<\/_name>/' \
	-i peg-e.appdata.xml.in

cd po
echo ' DONE'


echo -n 'Updating translations...'
for POFILE in *.po;
do
	echo -n " $POFILE"
	msgmerge --quiet --update --backup=none $POFILE description.pot
done
echo ' DONE'


echo -n 'Merging translations...'
cd ..

intltool-merge --quiet --desktop-style po peg-e.desktop.in peg-e.desktop
rm -f peg-e.desktop.in

intltool-merge --quiet --xml-style po peg-e.appdata.xml.in peg-e.appdata.xml
echo >> peg-e.appdata.xml
rm -f peg-e.appdata.xml.in

echo ' DONE'
