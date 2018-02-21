#pragma once

#include <gl/glut.h>
#include <iostream>
#include <atlImage.h>


namespace CsrTex {
	class CsrTexture {
	public:
		CsrTexture(GLenum TexTarget, const std::string& fileName)
		{

		}

		bool Load() 
		{

		}

		void Bind(GLenum TexUnit)
		{

		}

	private:
		CImage m_pImage;
		GLenum m_TextureTarget;
		GLuint m_TextureObj;
	};
}
