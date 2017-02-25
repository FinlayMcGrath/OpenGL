struct ShadowCaster
{
	std::vector<Vec3> verts; // shadow casting object verts
	Vec3 normal;	// normal
	GLuint shadowVolume; //display list for shadow volume
};
ShadowCaster shadowCaster;
	
void Scene3D::buildShadowVolume(ShadowCaster *caster, float lightPosit[3], float ext )
{
	if( caster->shadowVolume != -1 )
        glDeleteLists( caster->shadowVolume, 0 );

    caster->shadowVolume = glGenLists(1);

    glNewList( caster->shadowVolume, GL_COMPILE );
    {
        glDisable( GL_LIGHTING );

        glBegin( GL_QUADS );
        {
            glColor3f( 0.2f, 0.8f, 0.4f );

            float vExtended[3];

            //
            // For each vertex of the shadow casting object, find the edge 
            // that it helps define and extrude a quad out from that edge.
            //
            
            for( int i = 0; i < caster->verts.size(); i++ )
            {
                // Define the edge we're currently working on extruding...
                int e0 = i;
                int e1 = i+1;

                // If the edge's second vertex is out of array range, 
                // place it back at 0
                if( e1 >= caster->verts.size() )
                    e1 = 0;

                // v0 of our extruded quad will simply use the edge's first 
                // vertex or e0.
                glVertex3f( caster->verts[e0].getX(), 
                            caster->verts[e0].getY(), 
                            caster->verts[e0].getZ() );

                // v1 of our quad is created by taking the edge's first 
                // vertex and extending it out by some amount.
                extendVertex( vExtended, lightPosit, caster->verts[e0], ext );
                glVertex3f( vExtended[0], vExtended[1], vExtended[2] );

                // v2 of our quad is created by taking the edge's second 
                // vertex and extending it out by some amount.
                extendVertex( vExtended, lightPosit, caster->verts[e1], ext );
                glVertex3f( vExtended[0], vExtended[1], vExtended[2] );

                // v3 of our extruded quad will simply use the edge's second 
                // vertex or e1.
                glVertex3f( caster->verts[e1].getX(), 
                            caster->verts[e1].getY(), 
                            caster->verts[e1].getZ() );
            }
        }
		glColor3f( 1.0f, 1.0f, 1.0f );
        glEnd();

        glEnable( GL_LIGHTING );
    }
    glEndList();
}

void Scene3D::extendVertex( float newVert[3], float lightPosit[3], Vec3 vert, float ext )
{
    float lightDir[3];

    // Create a vector that points from the light's position to the original vertex.
    lightDir[0] = vert.getX() - lightPosit[0];
    lightDir[1] = vert.getY() - lightPosit[1];
    lightDir[2] = vert.getZ() - lightPosit[2];

    // Then use that vector to extend the original vertex out to a new position.
    // The distance to extend or extrude the new vector is specified by t.
    newVert[0] = lightPosit[0] + lightDir[0] * ext;
    newVert[1] = lightPosit[1] + lightDir[1] * ext;
    newVert[2] = lightPosit[2] + lightDir[2] * ext;
}