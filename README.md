# INI Parser Library

## Description
This library is designed to load and catalogue an INI style file to allow retrieval of values of varying types in an arbitrary fashion.

There is no "INI specification" as such, so this parser tries to be as flexible as possible.

## Syntax
Section definitions, key pairs and comments all appear on their own lines. Leading and trailing whitespace is ignored.

### Sections
The section name appears in square brackets. After the section definition, all keys are linked to that section. Sections finish at the very next section definition or the document’s end; there is no specific “end of section” separator. Section names must all be unique.

Example:

[Section_Name]

### Key pairs
An equals symbol separates the name and value of each key pair. To the left of the equals sign is the key name and to the right is its value. Key names must only appear once within a section. Any character can be used in the value including whitespace (except leading or trailing). Text values do not need to be quoted.

Examples:

Name_1 = Some value

Boolean = true

Integer = 42

### Comments
Comments are on a single line and proceeded with a hash and can contain any character after it.

Example:

\# This is a comment

### Notes
- Both section and key names are case insensitive and restricted to certain characters. No whitespace is allowed.

  [A-Za-z]+[A-Za-z0-9_]*

- Comments are not specifically handled. Although it's recommended to use a hash symbol, any line that is not a valid section definition nor a valid key pair, is ignored. This allows some flexibility in the character that's used to denote a comment. 

  Percent, dollar or ampersand symbols are examples that are valid.

- The value from INI_GetString will be lost after INI_Close.

  String duplication should be handled by the calling application.

- INI_Close only needs to be called after a successful INI_Open.

- Common value types are handled using the functions INI_GetString, INI_GetInteger and INI_GetBoolean. Other value types, such as float, can be added easily and at your discretion.

- If a string value needs to be quoted, the calling application should handle the removal of the quotes if necessary.

- Check the header file (ini.h) for further function usage.
