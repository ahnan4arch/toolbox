
#include <GL/glew.h>
#include <GL/gl.h>

#include "texture.h"
#include "../image/image.h"


namespace GL{

	unsigned TEXTURE::BINDER::lastBinded(0);
	TEXTURE::BINDER::BINDER(const TEXTURE& t) : prevBinded(lastBinded){
		if(lastBinded != t.tid){
			glBindTexture(GL_TEXTURE_2D, t.tid);
			lastBinded = t.tid;
		}
	}
	TEXTURE::BINDER::~BINDER(){
		if(prevBinded != lastBinded){
			glBindTexture(GL_TEXTURE_2D, prevBinded);
			lastBinded = prevBinded;
		}
	}



	const TEXTURE::PARAMS TEXTURE::defaultParams = {
		wrap_s : GL_REPEAT,
		wrap_t : GL_REPEAT,
		filter_mag : GL_LINEAR,
		filter_min : GL_LINEAR,
		texture_mode : GL_REPLACE,
		pointSprite : false,
	};

	TEXTURE::TEXTURE(const PARAMS& p) : tid(GetNewTID()), empty(true){}

	TEXTURE::TEXTURE(
		unsigned w,
		unsigned h,
		bool a,
		const PARAMS& p) : tid(GetNewTID()), empty(true){
		BINDER b(*this);
#if 0
		glTexStorage2D(GL_TEXTURE_2D, 0, a ? GL_RGBA : GL_RGB, w, h);
#else
		glTexImage2D(
			GL_TEXTURE_2D, 0,
			a ? GL_RGB : GL_RGBA,
			w, h, 0,
			GL_RGB,
			GL_UNSIGNED_BYTE, 0);
#endif
		SetupAttributes(p);
		empty = false;
	}

	TEXTURE::TEXTURE(
		const class IMAGE& image, const PARAMS& p) : tid(GetNewTID()), empty(true){
		Assign(image, p);
	}

	TEXTURE::~TEXTURE(){
		glDeleteTextures(1, &tid);
	}

	void TEXTURE::Assign(const IMAGE& image, const PARAMS& p){
		BINDER b(*this);

		const unsigned d(image.Depth());
		glTexImage2D(
			GL_TEXTURE_2D, 0,
			d <= 3 ? GL_RGB : GL_RGBA,
			image.Width(), image.Height(), 0,
			d <= 3 ? GL_BGR : GL_BGRA,
			GL_UNSIGNED_BYTE, image.Buffer());
		empty = false;

		//属性を設定
		SetupAttributes(p);
	}

	void TEXTURE::SetupAttributes(const PARAMS& p){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, p.wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, p.wrap_t);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,p.filter_mag);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,p.filter_min);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, p.texture_mode);

		if(p.pointSprite){
			glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
		}
	}

	void TEXTURE::Update(const IMAGE& image, int x, int y){
		if(empty){
			//テクスチャメモリが割り当てられていないので終了
			return;
		}
		BINDER b(*this);
		glTexSubImage2D(
			GL_TEXTURE_2D, 0,
			x,
			y,
			image.Width(),
			image.Height(),
			image.Depth()==4 ? GL_BGRA : GL_BGR,
			GL_UNSIGNED_BYTE,
			image.Buffer());
	}
	void TEXTURE::Update(
		const void* buffer, int format, int x, int y, unsigned width, unsigned height){
		if(empty){
			//テクスチャメモリが割り当てられていないので終了
			return;
		}
		BINDER b(*this);
		glTexSubImage2D(
			GL_TEXTURE_2D, 0,
			x,
			y,
			width,
			height,
			format,
			GL_UNSIGNED_BYTE,
			buffer);
	}

	unsigned TEXTURE::GetNewTID(){
		unsigned id;
		glGenTextures(1, &id);
		return id;
	}

}
