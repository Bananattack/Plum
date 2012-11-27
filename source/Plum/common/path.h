#pragma once

namespace plum
{
    // Path helpers.
    // Lifted from ika shamelessly
    namespace Path
    {
        // ifdef and blah blah for platform independance
        extern const char* delimeters;

        // strips the path from the filename, and returns it.
        // If relativeto is not specified or is empty, then the entire path is returned.
        // If it is specified, then the path returned is relative to the path contained therein.
        std::string getDirectory(const std::string& s);

        std::string getFileName(const std::string& s);           // same idea, but just returns the filename

        std::string getExtension(const std::string& s);          // returns the extension. (excluding the period)
        std::string replaceExtension(const std::string& s, const std::string& extension); // swaps the extension, and returns the result

        bool equals(const std::string& s, const std::string& t); // returns true if the two paths are the same.  Disregards case in win32.
    }
}