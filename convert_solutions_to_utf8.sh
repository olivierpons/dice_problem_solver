#!/bin/bash

# Check if source file exists
if [ ! -f "solutions.txt" ]; then
    echo "Error: solutions.txt file not found"
    exit 1
fi

# Create a copy of the original file
cp solutions.txt solutions_utf8.txt

# Perform replacements with sed
sed -i \
    -e 's/Hero/⚔️/g' \
    -e 's/Captain/🎖️/g' \
    -e 's/Peasant/👨‍🌾/g' \
    -e 's/Queen/👸/g' \
    -e 's/Soldier/🪖/g' \
    -e 's/Potter/⚱️/g' \
    -e 's/Scribe/✍️/g' \
    -e 's/Mage/🧙/g' \
    -e 's/Shaman/🪬/g' \
    -e 's/Traitor/🗡️/g' \
    -e 's/Thief/💰/g' \
    -e 's/Cursed/☠️/g' \
    solutions_utf8.txt

echo "Conversion complete. Result saved in solutions_utf8.txt"