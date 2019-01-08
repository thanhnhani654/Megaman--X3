#include "Sprite.h"

//Khai báo các biến dùng chung
LPDIRECT3DDEVICE9 Sprite::d3ddv;
LPDIRECT3DTEXTURE9 Sprite::_Image;
LPDIRECT3DTEXTURE9 Sprite::_Image2;
LPDIRECT3DTEXTURE9 Sprite::_Image3;
LPDIRECT3DTEXTURE9 Sprite::_Image4;
LPDIRECT3DTEXTURE9 Sprite::_Image5;
LPDIRECT3DTEXTURE9 Sprite::_Image6;
LPD3DXSPRITE Sprite::_SpriteHandler;

vector <SpritePos> Sprite::spritepos;
vector <AnimationClip> Sprite::animClips;

//Lấy biến chứa thông số của DirectX. Hàm chỉ cần chạy một lần
void Sprite::GetDevice(LPDIRECT3DDEVICE9 id3ddv)
{
	d3ddv = id3ddv;
}

//Khởi tạo các thông số cần thiết cho Sprite. Hàm chỉ cần chạy một lần, Vì các thông số dùng chung nên đã khởi tạo các thông số toàn cục.
LPDIRECT3DTEXTURE9 Sprite::LoadSprite(LPCWSTR Path, int Width, int Height)
{
	LPDIRECT3DTEXTURE9 _image;
	HRESULT result;
	//if (_SpriteHandler != nullptr)
	result = D3DXCreateSprite(d3ddv, &_SpriteHandler);

	D3DXIMAGE_INFO info;	

	if (result != DI_OK)
		return nullptr;

	result = D3DXGetImageInfoFromFile(Path, &info);

	result = D3DXCreateTextureFromFileEx(
		d3ddv,
		Path,
		info.Width,
		info.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(255, 255, 255),
		&info,
		NULL,
		&_image);
	return _image;
}

LPDIRECT3DTEXTURE9 Sprite::LoadSprite2(LPCWSTR Path, int Width, int Height)
{
	LPDIRECT3DTEXTURE9 _image;
	HRESULT result;
	//if (_SpriteHandler != nullptr)
	result = D3DXCreateSprite(d3ddv, &_SpriteHandler);

	D3DXIMAGE_INFO info;

	if (result != DI_OK)
		return nullptr;

	result = D3DXGetImageInfoFromFile(Path, &info);

	result = D3DXCreateTextureFromFileEx(
		d3ddv,
		Path,
		info.Width,
		info.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(50, 96, 166),
		&info,
		NULL,
		&_image);
	return _image;
}

void Sprite::Initialize()
{
	//Load Image
	_Image  = LoadSprite(TEXT("Resources/map.png"), 7933, 4078);
	_Image2 = LoadSprite2(TEXT("Resources/Megaman.png"), 947, 370);
	_Image3 = LoadSprite2(TEXT("Resources/Enemies.png"), 333, 363);
	_Image4 = LoadSprite2(TEXT("Resources/Other.png"), 594, 304);
	_Image5 = LoadSprite2(TEXT("Resources/BlastHornet.png"), 538, 308);
	_Image6 = LoadSprite(TEXT("Resources/map2.png"), 7933, 4078);
	//_Image5 = ...

	//Load Sprite
	LoadSpritePos("Resources/Sprite_Tiles_Sheet.xml");
	LoadSpritePos("Resources/Sprite_Character_Sheet.xml");
	LoadSpritePos("Resources/Sprite_Enemy_Sheet.xml");
	LoadSpritePos("Resources/Sprite_Other_Sheet.xml");
	LoadSpritePos("Resources/Sprite_BlastHornet_Sheet.xml");
	//LoadSpritePos("Resources/...");

	//Load Animation
	LoadAnimClips("Resources/Animation_Clip_Tiles.xml", _Image);
	LoadAnimClips("Resources/Animation_Clip_Character.xml", _Image2);
	LoadAnimClips("Resources/Animation_Clip_Enemy.xml", _Image3);
	LoadAnimClips("Resources/Animation_Clip_Other.xml", _Image4);
	LoadAnimClips("Resources/Animation_Clip_BlastHornet.xml", _Image5);
	LoadAnimClips("Resources/Animation_Clip_Tiles2.xml", _Image6);
	//LoadAnimClips("Resources/...", _Image5);
	
}

void Sprite::Render(float X, float Y)
{
	if (currentAnim.name == "null")
		return;

	if (currentSprite == nullptr)
	{
		//cout << "Null Animation.					Call From Render" <<endl;
		return;
	}

	HRESULT hr;

	RECT rect;
	rect.left = currentSprite->x;
	rect.top = currentSprite->y;
	rect.bottom = currentSprite->y + currentSprite->h;
	rect.right = currentSprite->x + currentSprite->w;

	_SpriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

	D3DXVECTOR3 position(X, Y, 0);

	D3DXMATRIX mt;
	D3DXMatrixIdentity(&mt);
	mt._11 = 1.0f;
	mt._22 = -1.0f;
	mt._41 = 0 - Camera::getInstance()->GetPosition().x;
	mt._42 = 220 + 25 + 16 +Camera::getInstance()->GetPosition().y;
	D3DXVECTOR4 vp_pos;

	D3DXVec3Transform(&vp_pos, &position, &mt);
	//World Position
	D3DXVECTOR3 p;

	//Flip
	int flipping = 1;
	D3DXMATRIX my;
	D3DXMatrixIdentity(&my);
	if (_Flip)
	{
		my._11 = -1.0f;	
		_SpriteHandler->SetTransform(&my);
		p = D3DXVECTOR3(vp_pos.x * (-1), vp_pos.y, 0);
	}
	else
	{
		my._11 = 1.0f;	
		_SpriteHandler->SetTransform(&my);
		p = D3DXVECTOR3(vp_pos.x, vp_pos.y, 0);
	}

	//offset
	D3DXVECTOR3 center;
	if (!customCenter)
		if (currentAnim.name != "map" && currentAnim.name != "map2")
			center = D3DXVECTOR3(currentSprite->w / 2, currentSprite->h / 2, 0);
		else
			center = D3DXVECTOR3(0, currentSprite->h, 0);
	else
	{
		switch (this->center)
		{
		case 1:
			center = D3DXVECTOR3(currentSprite->w, 0, 0);
			break;
		case 2:
			center = D3DXVECTOR3(0, 0, 0);
			break;
		case 3: 
			center = D3DXVECTOR3(currentSprite->w, currentSprite->h, 0);
			break;
		case 4:
			center = D3DXVECTOR3(0, currentSprite->h, 0);
			break;
		default:
			break;
		}
	}
	////////////////////////

	hr = _SpriteHandler->Draw(
		currentAnim.image,
		&rect,
		&center,
		&p,
		D3DCOLOR_XRGB(255, 255, 255)
	);
	_SpriteHandler->End();
	
	this->Next();
}

void Sprite::Next()
{
	if (stopAnimation)
		return;
	countFrame++;
	if (countFrame >= frameRate)
	{
		index++;
		countFrame = 0;
	}

	if (index >= currentAnim.clipCount)
	{
		if (bLoop)
			index = 0;
		else
			return;
	}
	currentSprite = &currentAnim.Clip[index];
	
}

void Sprite::LoadSpritePos(char* path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(path);

	if (!result)
	{
		MessageBox(NULL, TEXT("Sprite not found!"), TEXT("LoadSpritePos Error"), NULL);
	}

	pugi::xml_node tools = doc.child("data-set");

	int i = 0;

	SpritePos tempspritepos;

	for (pugi::xml_node tool = doc.first_child(); tool; tool = tool.next_sibling())
	{
		for (pugi::xml_node tool2 = tool.first_child(); tool2; tool2 = tool2.next_sibling())
		{
			//Get ID
			pugi::xml_node tool3 = tool2.first_child();
			tempspritepos.id = tool3.child_value();
			tool3 = tool3.next_sibling();

			// Get X
			tempspritepos.x = atoi(tool3.child_value());
			tool3 = tool3.next_sibling();

			//Get Y
			tempspritepos.y = atoi(tool3.child_value());
			tool3 = tool3.next_sibling();

			//Get W
			tempspritepos.w = atoi(tool3.child_value());
			tool3 = tool3.next_sibling();

			//Get H
			tempspritepos.h = atoi(tool3.child_value());
			tool3 = tool3.next_sibling();

			spritepos.push_back(tempspritepos);

			i++;
		}
	}
}

SpritePos Sprite::FindSpriteById(string id)
{
	SpritePos temp;
	temp.id = "null";
	for (std::vector<SpritePos>::iterator it = spritepos.begin(); it != spritepos.end(); it++)
	{
		if (id == (*it).id)
			return (*it);
	}
	return temp;
}

void Sprite::LoadAnimClips(char* path, LPDIRECT3DTEXTURE9 image)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(path);

	if (!result)
	{
		MessageBox(NULL, TEXT("Animation not found!"), TEXT("Load Anim Error"), NULL);
	}

	pugi::xml_node tools = doc.child("data-set");

	int i = 0;
	AnimationClip* tempAnimClip;

	for (pugi::xml_node tool = doc.first_child(); tool; tool = tool.next_sibling())
	{		
		for (pugi::xml_node tool2 = tool.first_child(); tool2; tool2 = tool2.next_sibling())
		{			
			tempAnimClip = new AnimationClip();
			//Get name
			pugi::xml_node tool3 = tool2.first_child();
			tempAnimClip->name = tool3.child_value();
			tool3 = tool3.next_sibling();

			//Get Clip Count
			tempAnimClip->clipCount = atoi(tool3.child_value());
			tool3 = tool3.next_sibling();

			for (int i = 0; i < 17; i++)
			{
				if (i < tempAnimClip->clipCount)
				{
					string id;
					id = tool3.child_value();
					tempAnimClip->Clip[i] = FindSpriteById(id);
				}

				tool3 = tool3.next_sibling();
				
			}

			tempAnimClip->image = image;

			//Get SpritePos
			animClips.push_back(*tempAnimClip);
		}
	}
}

AnimationClip Sprite::FindAnimByName(string name)
{
	AnimationClip temp;
	temp.name = "null";
	for (std::vector<AnimationClip>::iterator it = animClips.begin(); it != animClips.end(); it++)
	{
		if (name == (*it).name)
			return (*it);
	}

	return temp;
}

void Sprite::SetAnimation(string name, bool isLoop)
{	
	AnimationClip temp;
	//temp = FindAnimByName(name);
	this->currentAnim = FindAnimByName(name);
	//if (temp.name == "null")
	if (currentAnim.name == "null")
	{
		cout << "Set Animation Error. Wrong Animation name!";
		return;
	}
	//currentAnim = temp;

	//currentAnim.name = "Walk";

	index = 0;
	currentSprite = &currentAnim.Clip[index];
	SetFrameRate(5);
	bLoop = isLoop;
}

void Sprite::SetFrameRate(int fr)
{
	frameRate = fr;
	countFrame = 0;
}

void Sprite::SetCenter(int i)
{
	this->center = i;
	customCenter = true;
}

void Sprite::ToggleAnimationClip()
{
	stopAnimation = !stopAnimation;
}

void Sprite::PrintSpritePos(int mode)
{
	cout << "========================================================================================================================" << endl;
	switch (mode)
	{
	case 1:
		for (int i = 0; i < spritepos.size(); i++)
		{
			cout << "ID: " << "\t" << "\t" << spritepos[i].id << endl;
			cout << "X: " << "\t" << "\t" << spritepos[i].x << endl;
			cout << "Y: " << "\t" << "\t" << spritepos[i].y << endl;
			cout << "Width: " << "\t" << "\t" << spritepos[i].w << endl;
			cout << "Height: " << "\t" << spritepos[i].h << endl << endl;
		}
		break;
	case 2:
		cout << "ID\t\tX\t\tY\t\tWidth\t\tHeight" << endl << endl;
		for (int i = 0; i < spritepos.size(); i++)
		{
			cout << spritepos[i].id << "\t\t" << spritepos[i].x << "\t\t" << spritepos[i].y << "\t\t" << spritepos[i].w << "\t\t" << spritepos[i].h << endl;
		}
		break;
	}
	cout << "========================================================================================================================" << endl << endl;
}

void Sprite::PrintAnimationClip(int mode)
{
	cout << "========================================================================================================================" << endl;
	switch (mode)
	{
	case 1:
		for (int i = 0; i < animClips.size(); i++)
		{
			cout << "Name: " << "\t" << "\t" << animClips[i].name << endl;
			cout << "Clip Count: " << "\t" << "\t" << animClips[i].clipCount << endl;
			for (int j = 0; j < 17; j++)
			{
				cout << "Clip "<< j << ": " << "\t" << "\t" << animClips[i].Clip[j].id << endl;
			}
			cout << endl;
		}
		break;
	case 2:
		cout << "Name\tClip Count\tClip1\tClip2\tClip3\tClip4\tClip5\tClip6\tClip7\tClip8\tClip9\tClip10" << endl << endl;
		for (int i = 0; i < animClips.size(); i++)
		{
			cout << animClips[i].name << "\t" << animClips[i].clipCount << "\t";
			for (int j = 0; j < animClips[i].clipCount; j++)
			{
				cout << "\t" << animClips[i].Clip[j].id;
			}
			cout << endl;
		}
		break;
	}
	cout << "========================================================================================================================" << endl << endl;
}

void Sprite::FlipLeft()
{
	_Flip = true;
}

void Sprite::FlipRight()
{
	_Flip = false;
}

bool Sprite::isFilpLeft()				
{
	return _Flip;
}