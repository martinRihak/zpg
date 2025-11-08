const float plane[] = {
    // První trojúhelník
    -10.0f, 0.0f, -10.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,   // Levý dolní roh
     10.0f, 0.0f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 0.0f,  // Pravý dolní roh
     10.0f, 0.0f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f, // Pravý horní roh
    
    // Druhý trojúhelník
    -10.0f, 0.0f, -10.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,   // Levý dolní roh
     10.0f, 0.0f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f, // Pravý horní roh
    -10.0f, 0.0f,  10.0f,  0.0f, 1.0f, 0.0f,  0.0f, 10.0f   // Levý horní roh
};