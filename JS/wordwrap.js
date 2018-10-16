if (process.argv.length < 3)
{
  console.log("Please specify file name");
  return;
}

var fileName = process.argv[2];

var fs = require('fs');

console.time('read_file');

if (!fs.existsSync(fileName))
{
    console.log("File not found");
    return;
}

var text = fs.readFileSync(fileName, 'ascii');

console.timeEnd('read_file');

var lineStartIndices = [];
// assume characters have the same widths but will use universal approach
const charWidth = 1;
const textWidth = 30;

const separators = [' ', '\t', '\r'];
const newLine = '\n';

console.time('process_text');

var lastPossibleLineStart = 0;
var currWidth = 0;
var wasBlank = false;
var lastWordStart = 0;
var lastWordWidth = 0;
var wordStarted = false;
            
var j = 0;
// introducing the variable decreses time on 30%
var length = text.length;
while (j < length)
{
   var c = text[j];
   var isBlank = false;
   for (var x = 0; x < separators.length; x++ )
   {
     if (separators[x] == c)
     {
       isBlank = true;
       break;
     }
   }

   currWidth += charWidth;

    if (wasBlank)
    {
       if (!isBlank) { lastWordStart = j; lastWordWidth = 1; wordStarted = true; }
    }
    else
       if (isBlank) { wordStarted = false; }

       lastWordWidth += charWidth;

       if ((wordStarted && currWidth > textWidth) || j >= length - 1 || c == newLine)
       {
          lineStartIndices.push(lastPossibleLineStart);
          lastPossibleLineStart = lastWordStart;
          currWidth = lastWordWidth;
          wordStarted = false;
       }
    wasBlank = isBlank;
    j++;
}

console.timeEnd('process_text');

// write first 50 lines

for (var i = 0; i < 50 && i < lineStartIndices.length; i++ )
{
    var lineStart = lineStartIndices[i];
    var lineEnd = i == lineStartIndices.length - 1 ? text.length-1 : lineStartIndices[i+1]-1
    console.log(text.substring(lineStart, lineEnd));
}

