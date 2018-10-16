using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Diagnostics;

namespace WordWrapTest
{
    class Program
    {
        static void Main(string[] args)
        {
            var w = new Stopwatch();
            w.Start();

            if (args.Length == 0)
            {
                Console.WriteLine("Please specify file name");
                return;
            }

            var fileName = args[0];

            if (!File.Exists(fileName))
            {
                Console.WriteLine("File not found");
                return;
            }

            string text;
            using (StreamReader sr = new StreamReader(fileName))
            {
                text = sr.ReadToEnd();
            }
            w.Stop();
            Console.WriteLine("Reading from file: size=[{0}], time=[{1}] msec", text.Length, w.ElapsedMilliseconds);


            var lineStartIndices = new List<int>();
            // assume characters have the same widths but will use universal approach
            const int charWidth = 1;
            const int textWidth = 30;
            // change to hashset from array decrease the time 2 times
            var separators = new char[] { ' ', '\t', '\r'};
            char newLine = '\n';

            w.Restart();

            int lastPossibleLineStart = 0;
            int currWidth = 0;
            bool wasBlank = false;
            int lastWordStart = 0;
            int lastWordWidth = 0;
            bool wordStarted = false;
            
            int j = 0;
            // introducing the variable decreses time on 30%
            int length = text.Length;
            while (j < length)
            {
                var c = text[j];
                //bool isBlank = separators.Contains(c);
                bool isBlank = false;
                for (int x = 0; x < separators.Length; x++ )
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
                    lineStartIndices.Add(lastPossibleLineStart);
                    lastPossibleLineStart = lastWordStart;
                    currWidth = lastWordWidth;
                    wordStarted = false;
                }
                wasBlank = isBlank;
                j++;
            }

            
            w.Stop();
            Console.WriteLine("Cycle time=[{0}], lines=[{1}]", w.ElapsedMilliseconds, lineStartIndices.Count);

            for (int i = 0; i < 50 && i < lineStartIndices.Count; i++ )
            {
                var lineStart = lineStartIndices[i];
                var lineLength = i < lineStartIndices.Count - 1 ? lineStartIndices[i+1] - lineStartIndices[i] : text.Length - lineStartIndices[i];
                Console.WriteLine(text.Substring(lineStartIndices[i], lineLength));
            }

                Console.ReadLine();
        }
    }
}
