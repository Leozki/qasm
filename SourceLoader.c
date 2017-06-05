//
// Created by Leozki on 2017/6/5.
//

#include "SourceLoader.h"
#include "Constants.h"
#include "StringUtils.h"

void LoadSourceFile ()
{

    if ( ! ( g_pSourceFile = fopen ( g_pstrSourceFilename, "rb" ) ) )
        ExitOnError ( "Could not open source file" );

    // Count the number of source lines

    while ( ! feof ( g_pSourceFile ) )
        if ( fgetc ( g_pSourceFile ) == '\n' )
            ++ g_iSourceCodeSize;
    ++ g_iSourceCodeSize;

    // Close the file

    fclose ( g_pSourceFile );

    // Reopen the source file in ASCII mode

    if ( ! ( g_pSourceFile = fopen ( g_pstrSourceFilename, "r" ) ) )
        ExitOnError ( "Could not open source file" );

    // Allocate an array of strings to hold each source line

    if ( ! ( g_ppstrSourceCode = ( char ** ) malloc ( g_iSourceCodeSize * sizeof ( char * ) ) ) )
        ExitOnError ( "Could not allocate space for source code" );

    // Read the source code in from the file

    for ( int iCurrLineIndex = 0; iCurrLineIndex < g_iSourceCodeSize; ++ iCurrLineIndex )
    {
        // Allocate space for the line

        if ( ! ( g_ppstrSourceCode [ iCurrLineIndex ] = ( char * ) malloc ( MAX_SOURCE_LINE_SIZE + 1 ) ) )
            ExitOnError ( "Could not allocate space for source line" );

        // Read in the current line

        fgets ( g_ppstrSourceCode [ iCurrLineIndex ], MAX_SOURCE_LINE_SIZE, g_pSourceFile );

        // Strip comments and trim whitespace

        StripComments ( g_ppstrSourceCode [ iCurrLineIndex ] );
        TrimWhitespace ( g_ppstrSourceCode [ iCurrLineIndex ] );

        // Make sure to add a new newline if it was removed by the stripping of the
        // comments and whitespace. We do this by checking the character right before
        // the null terminator to see if it's \n. If not, we move the terminator over
        // by one and add it. We use strlen () to find the position of the newline
        // easily.

        int iNewLineIndex = strlen ( g_ppstrSourceCode [ iCurrLineIndex ] ) - 1;
        if ( g_ppstrSourceCode [ iCurrLineIndex ] [ iNewLineIndex ] != '\n' )
        {
            g_ppstrSourceCode [ iCurrLineIndex ] [ iNewLineIndex + 1 ] = '\n';
            g_ppstrSourceCode [ iCurrLineIndex ] [ iNewLineIndex + 2 ] = '\0';
        }
    }

    // Close the source file

    fclose ( g_pSourceFile );
}