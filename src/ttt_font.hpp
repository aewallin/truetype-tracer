struct ttt_font {
    // #define TTFONT "/usr/share/fonts/truetype/freefont/FreeSerifBoldItalic.ttf"
    //#define TTFONT "/usr/share/fonts/truetype/freefont/FreeSerif.ttf"
    ttt_font() {
        
        fonts.push_back( "/usr/share/fonts/truetype/freefont/FreeSerif.ttf" );
        fonts.push_back( "/usr/share/fonts/truetype/freefont/FreeSerifBold.ttf" );
        fonts.push_back( "/usr/share/fonts/truetype/freefont/FreeSerifItalic.ttf" );
        fonts.push_back( "/usr/share/fonts/truetype/freefont/FreeSerifBoldItalic.ttf" );
        
        fonts.push_back( "/usr/share/fonts/truetype/freefont/FreeMono.ttf" );
        fonts.push_back( "/usr/share/fonts/truetype/freefont/FreeMonoBold.ttf" );
        fonts.push_back( "/usr/share/fonts/truetype/freefont/FreeMonoBoldOblique.ttf" );
        fonts.push_back( "/usr/share/fonts/truetype/freefont/FreeMonoOblique.ttf" );
        
        fonts.push_back( "/usr/share/fonts/truetype/freefont/FreeSans.ttf" );
        fonts.push_back( "/usr/share/fonts/truetype/freefont/FreeSansBold.ttf" );
        fonts.push_back( "/usr/share/fonts/truetype/freefont/FreeSansBoldOblique.ttf" );
        fonts.push_back( "/usr/share/fonts/truetype/freefont/FreeSansOblique.ttf" );
        
    }
    std::string font_path(int i) {
        return fonts[i];
    }
    std::string operator[](unsigned int i) {
        return fonts[i];
    }
    std::vector< std::string > fonts;
};
