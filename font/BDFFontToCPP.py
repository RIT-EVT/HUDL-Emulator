#
# A quick and dirty python script to convert a .bdf font file into a cpp file
# that is recongizable by the displays we use. This script processes the BDF font file
# and then flips the characters to account for the vertical 8 bit segments instead of
# instead of horizontal 8 bit segments.
#
# Author: Zachary Lineman
#

import numpy as np

with open("./tom-thumb.bdf", encoding="utf-8") as f:
    numberOfBytes = 6;
    scanningBitmap = False;
    currentBytes = []
    currentBitmap = ""

    for line in f:
        line = line.strip()
        if line == "BITMAP":
            scanningBitmap = True
        elif line == "ENDCHAR":
            scanningBitmap = False

            front = False
            length = len(currentBytes)

            while length < numberOfBytes:
                if front:
                    currentBytes.insert(0, "00000000")
                else:
                    currentBytes.append("00000000")
                front = not front
                length = len(currentBytes)

            characterMatrix = np.empty((6, 8))
            x = 0
            for byte in currentBytes:
                binaryArray = []
                
                for bit in byte:
                    binaryArray.append(bit)

                characterMatrix[x] = binaryArray
                x += 1

            characterMatrix = np.rot90(characterMatrix, 3)
            
            characterComment = currentBitmap
            currentBitmap = "{ "
            for row in characterMatrix:
                currentByte = ""
                for column in row:
                    currentByte += str ( int(column) ).replace(".", "")

                currentByte = f'0x{int(currentByte, 2):X}'
                currentBitmap += currentByte + ","
            currentBitmap += " }," + characterComment
            print(currentBitmap)
            currentBitmap = ""

            currentBytes = []
        elif "STARTCHAR" in line:
            line = line.replace("STARTCHAR ", "")
            currentBitmap = f"// Character: {line}"
        elif scanningBitmap:
            intLine = int( line, 16 )
            currentBytes.append(f'{intLine:0>8b}')
