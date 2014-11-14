#!/bin/sh


echo -n 'Preparing files...'
cd ..

rm -f peg-e.desktop.in.h
rm -f peg-e.desktop.in
cp peg-e.desktop peg-e.desktop.in
sed -e '/^Name\[/ d' \
	-e '/^GenericName\[/ d' \
	-e '/^Comment\[/ d' \
	-e 's/^Name/_Name/' \
	-e 's/^GenericName/_GenericName/' \
	-e 's/^Comment/_Comment/' \
	-i peg-e.desktop.in
intltool-extract --quiet --type=gettext/ini peg-e.desktop.in

rm -f peg-e.appdata.xml.in.h
rm -f peg-e.appdata.xml.in
cp peg-e.appdata.xml peg-e.appdata.xml.in
sed -e '/p xml:lang/ d' \
	-e '/summary xml:lang/ d' \
	-e 's/<p>/<_p>/' \
	-e 's/<\/p>/<\/_p>/' \
	-e 's/<summary>/<_summary>/' \
	-e 's/<\/summary>/<\/_summary>/' \
	-i peg-e.appdata.xml.in
intltool-extract --quiet --type=gettext/xml peg-e.appdata.xml.in

cd po
echo ' DONE'


echo -n 'Extracting messages...'
xgettext --from-code=UTF-8 --c++ --keyword=_ --keyword=N_:1 \
	--package-name='Peg-E' --copyright-holder='Graeme Gott' \
	--output=description.pot ../*.h
sed 's/CHARSET/UTF-8/' -i description.pot
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
rm -f peg-e.desktop.in.h
rm -f peg-e.desktop.in

intltool-merge --quiet --xml-style po peg-e.appdata.xml.in peg-e.appdata.xml
rm -f peg-e.appdata.xml.in.h
rm -f peg-e.appdata.xml.in

echo ' DONE'
