#include "model.h"


bool Model::Load(char* modelFilename, char* textureFilename)
{
	bool result;

	// Load in the model data,
	result = LoadModel(modelFilename);
	if(!result)
	{
		MessageBox(NULL, "Model failed to load", "Error", MB_OK);
		return false;
	}

	// Load the texture for this model.
	LoadTexture(textureFilename);
	
	return true;
}

void Model::render()
{
	//renders model based upon data loaded
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer (3, GL_FLOAT, 0, vertex.data());
	glNormalPointer (GL_FLOAT, 0, normals.data());
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords.data());

	glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

bool Model::LoadModel(char* filename)
{
	std::ifstream fileStream;
    int fileSize = 0;

    fileStream.open( filename, std::ifstream::in );
   
    if( fileStream.is_open() == false )
        return false;

    fileStream.seekg( 0, std::ios::end );
    fileSize = ( int )fileStream.tellg( );
    fileStream.seekg( 0, std::ios::beg );

    if( fileSize <= 0 )
        return false;

    char *buffer = new char[fileSize];

    if( buffer == 0 )
        return false;

    memset( buffer, '\0', fileSize );

    TokenStream tokenStream, lineStream, faceStream;
    string tempLine, token;

    fileStream.read( buffer, fileSize );
    tokenStream.SetTokenStream( buffer );

    delete[] buffer;

    tokenStream.ResetStream( );

	float tempx, tempy, tempz;
    vector<Vec3> verts, norms, texC;
    vector<int> faces;
	

    char lineDelimiters[2] = { '\n', ' ' };

    while( tokenStream.MoveToNextLine( &tempLine ) )
    {
        lineStream.SetTokenStream( ( char* )tempLine.c_str( ) );
        tokenStream.GetNextToken( 0, 0, 0 );

        if( !lineStream.GetNextToken( &token, lineDelimiters, 2 ) )
            continue;

        if( strcmp( token.c_str( ), "v" ) == 0 )
        {
            lineStream.GetNextToken( &token, lineDelimiters, 2 );
			tempx = (float)atof(token.c_str());

            lineStream.GetNextToken( &token, lineDelimiters, 2 );
			tempy = (float)atof(token.c_str());

            lineStream.GetNextToken( &token, lineDelimiters, 2 );
			tempz = (float)atof(token.c_str());

			verts.push_back(Vec3(tempx, tempy, tempz));
        }
        else if( strcmp( token.c_str( ), "vn" ) == 0 )
        {
            lineStream.GetNextToken( &token, lineDelimiters, 2 );
			tempx = (float)atof(token.c_str());

            lineStream.GetNextToken( &token, lineDelimiters, 2 );
			tempy = (float)atof(token.c_str());

            lineStream.GetNextToken( &token, lineDelimiters, 2 );
			tempz = (float)atof(token.c_str());

			norms.push_back(Vec3(tempx, tempy, tempz));
        }
        else if( strcmp( token.c_str( ), "vt" ) == 0 )
        {
            lineStream.GetNextToken( &token, lineDelimiters, 2 );
			tempx = (float)atof(token.c_str());

            lineStream.GetNextToken( &token, lineDelimiters, 2 );
			tempy = (float)atof(token.c_str());

			texC.push_back(Vec3(tempx, tempy, 0));
        }
        else if( strcmp( token.c_str( ), "f" ) == 0 )
        {
            char faceTokens[3] = { '\n', ' ', '/' };
            std::string faceIndex;

            faceStream.SetTokenStream( ( char* )tempLine.c_str( ) );
            faceStream.GetNextToken( 0, 0, 0 );

            for( int i = 0; i < 3; i++ )
            {
                faceStream.GetNextToken( &faceIndex, faceTokens, 3 );
                faces.push_back( ( int )atoi( faceIndex.c_str( ) ) );

                faceStream.GetNextToken( &faceIndex, faceTokens, 3 );
                faces.push_back( ( int )atoi( faceIndex.c_str( ) ) );

                faceStream.GetNextToken( &faceIndex, faceTokens, 3 );
                faces.push_back( ( int )atoi( faceIndex.c_str( ) ) );
            }
        }
        else if( strcmp( token.c_str( ), "#" ) == 0 )
        {
           //skip
        }

        token[0] = '\0';
    }

    // "Unroll" the loaded obj information into a list of triangles.
	
    int numFaces = (int)faces.size() / 9;
	m_vertexCount = numFaces * 3;
	
	// Model data is stored in stored in vectors verts, norms, texC and faces
	//set the vertex, norms and texture coordinates based upon which face it is
	
	for (int i = 0; i < (int)faces.size(); i += 3)
	{
		vertex.push_back(verts[faces[i] - 1].getX());
		vertex.push_back(verts[faces[i] - 1].getY());
		vertex.push_back(verts[faces[i] - 1].getZ());
		
		texCoords.push_back(texC[faces[i + 1] - 1].getX());
		texCoords.push_back(texC[faces[i + 1] - 1].getY());

		normals.push_back(norms[faces[i + 2] - 1].getX());
		normals.push_back(norms[faces[i + 2] - 1].getY());
		normals.push_back(norms[faces[i + 2] - 1].getZ());
		
	}
	

	//delete the data
    verts.clear( );
    norms.clear( );
    texC.clear( );
    faces.clear( );

    return true;
}

void Model::LoadTexture(char* filename)
{	
	texture = SOIL_load_OGL_texture
	(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y
	);

	//check for an error during the load process
	if(texture==0 )
	{
		printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
	}
}