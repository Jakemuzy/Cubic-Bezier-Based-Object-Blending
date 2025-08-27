Cubic Bezier Based Object Interpolation

The purpose of this demonstration is to provide a new way to blend objects together. 

Wheras other methods have a static blending factor, the new method allows for user 
to specify how the objects blend, and when they start blending

The structure of this program is split into the following steps as described below

OBJECT INTERSECTION
    -   First, in order to determine the blend factor we must first find whether 
        or not objeccts are intersecting eachother
        -   Objects are first split into octree representations and their 
            bounding boxes are compared
        -   Each octree child node contains the vertices of the parent object, and 
            for every node that intersects a node of another object, the 
            vertices are then tested

NEARTEST VERTEX DETERMINATION
    -   In order to find what to blend, each intersecting octree node finds its nearest
        vertex neighbor from the intersecting object's octree vertices
        -   A midpoint between the two points is then found and added to the new object
        -   This allows for the blending to next step to take place

CUBIC BEZIER CURVE SAMPLING
    -   Second, the blending factor is sampled based on a user specified Bezier Curve
        -   The Bezier Curve is provided by a user, and is thus used to determine
            how fast or where the blending is achieved.
        -   The Bezier curve is sampled as a function of distance of the midpoint
            compared to the whole length of the octree intersection bounds

NEW OBJECT BASED ON INTERSECTION
    -   Both original objects are hidden while an intersection remains between the 
        two original objects. These objects temportrily only serve the purpose of 
        determining on how the new object looks / is blended
        -   A new object is created combining the vertices contained in the
            non intersecting octree nodes, and the weighted average of the new 
            verices in the intersecting octree nodes
        -   
    -   Once the intersecting objects are no longer intersecting, the new object is 
        delted and the the original objects are no longer hidden




Architecture 
    Main ->
        unique_ptr Scene

    ServiceProvider -> 
        -> Init Graphics Renderer (OpenGL Wrapper)
        -> Init Input Handler 
        -> Init Camera 
        -> Init Scene 