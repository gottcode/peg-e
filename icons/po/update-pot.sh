#!/bin/sh


echo -n 'Preparing files...'
cd ..

rm -f peg-e.desktop.in
cp peg-e.desktop peg-e.desktop.in
sed -e '/^Name\[/ d' \
	-e '/^GenericName\[/ d' \
	-e '/^Comment\[/ d' \
	-e '/^Icon/ d' \
	-e '/^Keywords/ d' \
	-i peg-e.desktop.in

rm -f peg-e.appdata.xml.in
cp peg-e.appdata.xml peg-e.appdata.xml.in
sed -e '/p xml:lang/ d' \
	-e '/summary xml:lang/ d' \
	-e '/name xml:lang/ d' \
	-e '/<developer_name>/ d' \
	-i peg-e.appdata.xml.in

cd po
echo ' DONE'


echo -n 'Extracting messages...'
xgettext --from-code=UTF-8 --output=description.pot \
	--package-name='Peg-E' --copyright-holder='Graeme Gott' \
	../*.in
sed 's/CHARSET/UTF-8/' -i description.pot
echo ' DONE'


echo -n 'Cleaning up...'
cd ..

rm -f peg-e.desktop.in
rm -f peg-e.appdata.xml.in

echo ' DONE'
