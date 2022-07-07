#include "glyph.h"

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
Glyph::Glyph():
   img(),
   width(0),
   height(0),
   dx(0),
   dy(0),
   xAdvance(0),
   yAdvance(0)
{

}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
QDebug operator<<(QDebug stream, const Glyph &gl)
{
     stream.nospace() << "symb: " << QString(QChar(gl.key)) << " size (" << gl.width << ", " << gl.height << ")" <<
                     ", dx:dy " << gl.dx << ":" << gl.dy << ", advance [" << gl.xAdvance << ", " << gl.yAdvance << "]";
     return stream.maybeSpace();
}
