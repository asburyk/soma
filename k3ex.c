#include <stdio.h>
//
int codes[70];

int shapes[15] = {3, 5, 6, 9, 10, 12, 17, 18, 20, 24, 33, 34, 36, 40, 48};

struct pos {
    int x;
    int y;
    int z;
};

struct perm {
    struct pos p[8];
};

struct perm4 { 
    int posCode;
    struct pos p[4];
};


void calcCodes() {
    int i = 0;
    for (int code = 0; code < 256; code++) {
	if ( (code % 2) + (code/2 % 2) + (code/4 % 2) + (code/8 % 2) + (code/16 % 2) + (code/32 % 2) + (code/64 % 2) + (code/128 % 2) == 4 ) {
	   codes[i] = code;
	   i++;
	}
    }
}

int assignCode(int permCode, int kcode, struct perm4 *p1, struct perm4 *p2) {
    int i1 = 0;
    int i2 = 0;
    for (int i = 0; i < 27; i++) {
	if (permCode % 2) {
	    if (kcode % 2) {
		p2->posCode = i;
		p2->p[i2].x = i % 3;
		p2->p[i2].y = i / 3 % 3;
		p2->p[i2].z = i / 9 % 3;
		i2++;
	    } else {
		p1->posCode = i;
		p1->p[i1].x = i % 3;
		p1->p[i1].y = i / 3 % 3;
		p1->p[i1].z = i / 9 % 3;
		i1++;
	    }
	    kcode /= 2;
	}
	permCode /= 2;
    }
    return !((i1 == 4) && (i2 == 4));
}

int assignCodeBrown(int permCode, int kcode, struct perm4 *p1, struct perm4 *pB) {
    int i1 = 0;
    int i2 = 0;
    for (int i = 0; i < 27; i++) {
	if (permCode % 2) {
	    if (kcode % 2) {
		pB->posCode = i;
		pB->p[i2].x = i % 3;
		pB->p[i2].y = i / 3 % 3;
		pB->p[i2].z = i / 9 % 3;
		i2++;
	    } else {
		p1->posCode = i;
		p1->p[i1].x = i % 3;
		p1->p[i1].y = i / 3 % 3;
		p1->p[i1].z = i / 9 % 3;
		i1++;
	    }
	    kcode /= 2;
	}
	permCode /= 2;
    }
    pB->p[3].x = pB->p[0].x;
    pB->p[3].y = pB->p[0].y;
    pB->p[3].z = pB->p[0].z;
    return (i2 == 3) && (i1 == 4);

}

// Returns 0 on false
// Returns 1 for x-plane
// Returns 2 for y-plane
// Returns 3 for z-plane
int isPlane(struct perm4 p, int *v) {
    if (p.p[0].x == p.p[1].x && p.p[1].x == p.p[2].x && p.p[2].x == p.p[3].x) {
	*v = p.p[0].x;
	return 1;
    }
    if (p.p[0].y == p.p[1].y && p.p[1].y == p.p[2].y && p.p[2].y == p.p[3].y) {
	*v = p.p[0].y;
	return 2;
    }
    if (p.p[0].z == p.p[1].z && p.p[1].z == p.p[2].z && p.p[2].z == p.p[3].z) {
	*v = p.p[0].z;
	return 3;
    }
    return 0;
}

// Returns 0 on false
// Returns 1 on x-line
// Returns 2 on y-line
// Returns 3 on z-line
// Never returns plane (i.e., the nonzero return from isPlane should be put in plane)
int is3Line(struct perm4 p, int plane, int *v) {
    int x[3] = {0, 0 ,0};
    int y[3] = {0, 0, 0};
    int z[3] = {0, 0, 0};
    if (plane != 1) {
	for (int i = 0; i < 4; i++) {
	    x[p.p[i].x]++;
	}
	for (int i = 0; i < 3; i++) {
	    if (x[i] == 3) {
		*v = i;
		return 1;
	    }
	}
    }
    if (plane != 2) {
	for (int i = 0; i < 4; i++) {
	    y[p.p[i].y]++;
	}
	for (int i = 0; i < 3; i++) {
	    if (y[i] == 3) {
		*v = i;
		return 2;
	    }
	}
    }
    if (plane != 3) {
	for (int i = 0; i < 4; i++) {
	    z[p.p[i].z]++;
	}
	for (int i = 0; i < 3; i++) {
	    if (z[i] == 3) {
		*v = i;
		return 3;
	    }
	}
    }
    return 0;
}

// Returns 0 on false
// Returns 1 on true
int is22Line(struct perm4 p, int plane) {
    int a[3] = {0, 0, 0};
    int b[3] = {0, 0, 0};
    if (plane == 1) { // a=y, b=z
	for (int i = 0; i < 4; i++) {
	    a[p.p[i].y]++;
	    b[p.p[i].z]++;
	}
    } 
    if (plane == 2) { // a=x, b=z
	for (int i = 0; i < 4; i++) {
	    a[p.p[i].x]++;
	    b[p.p[i].z]++;
	}
    } 
    if (plane == 3) { // a=x, b=y
	for (int i = 0; i < 4; i++) {
	    a[p.p[i].x]++;
	    b[p.p[i].y]++;
	}
    }
    return (a[1] == b[1]) && (b[1] == 2) && ((a[0] == 1) ^ (b[0] == 1));
}


// Returns -1 on false
// Returns pos index on match
int isIn(struct perm4 p, struct pos s) {
    for (int i = 0; i < 4; i++) {
	if (p.p[i].x == s.x && p.p[i].y == s.y && p.p[i].z == s.z)
	    return i;
    }
    return -1;
}


// All piece checks
// Return 0 on false
// Return 1 on true

int isYellow(struct perm4 p) {
    int pv = -1;
    int plane = isPlane(p, &pv);
    if (plane == 0)
	return 0;
    int lv = -1;
    int line = is3Line(p, plane, &lv);
    if (line == 0)
	return 0;
    struct pos s;
    if (plane == 1) {
	s.x = pv;
	if (line == 2) {
	    s.y = lv;
	    if (s.y - 1 >= 0) {
		s.y = s.y - 1;
		s.z = 0;
		if (isIn(p, s) != -1)
		    return 1;
		s.z = 2;
		if (isIn(p, s) != -1)
		    return 1;
	    }
	    s.y = lv;
	    if (s.y + 1 <= 2) {
		s.y = s.y + 1;
		s.z = 0;
		if (isIn(p, s) != -1)
		    return 1;
		s.z = 2;
		if (isIn(p, s) != -1)
		    return 1;
	    }
	} else {
	    s.z = lv;
	    if (s.z - 1 >= 0) {
		s.z = s.z - 1;
		s.y = 0;
		if (isIn(p, s) != -1)
		    return 1;
		s.y = 2;
		if (isIn(p,s) != -1)
		    return 1;
	    }
	    s.z = lv;
	    if (s.z + 1 <= 2) {
		s.z = s.z + 1;
		s.y = 0;
		if (isIn(p,s) != -1)
		    return 1;
		s.y = 2;
		if (isIn(p,s) != -1)
		    return 1;
	    }
	}
    }
    if (plane == 2) {
	s.y = pv;
	if (line == 1) {
	    s.x = lv;
	    if (s.x -1 >= 0) {
		s.x = s.x - 1;
		s.z = 0;
		if (isIn(p,s) != -1)
		    return 1;
		s.z = 2;
		if (isIn(p, s) != -1)
		    return 1;
	    }
	    s.x = lv;
	    if (s.x + 1 <= 2) {
		s.x = s.x + 1;
		s.z = 0;
		if (isIn(p,s) != -1)
		    return 1;
		s.z = 2;
		if (isIn(p,s) != -1)
		    return 1;
	    }
	} else {
	    s.z = lv;
	    if (s.z - 1 >= 0) {
		s.z = s.z - 1;
		s.x = 0;
		if (isIn(p,s) != -1)	
		    return 1;
		s.x = 2;
		if (isIn(p,s) != -1)
		    return 1;
	    }
	    s.z = lv;
	    if (s.z + 1 <= 2) {
		s.z = s.z + 1;
		s.x = 0;
		if (isIn(p,s) != -1)
		    return 1;
		s.x = 2;
		if (isIn(p,s) != -1)
		    return 1;
	    }
	}
    }
    if (plane == 3) {
	s.z = pv;
	if (line == 1) {
	    s.x = lv;
	    if (s.x - 1 >= 0) {
		s.x = s.x - 1;
		s.y = 0;
		if (isIn(p,s) != -1)
		    return 1;
		s.y = 2;
		if (isIn(p,s) != -1)
		    return 1;
	    }
	    s.x = lv;
	    if (s.x + 1 <= 2) {
		s.x = s.x + 1;
		s.y = 0;
		if (isIn(p,s) != -1)
		    return 1;
		s.y = 2;
		if (isIn(p,s) != -1)
		    return 1;
	    }
	} else {
	    s.y = lv;
	    if (s.y - 1 >= 0) {
		s.y = s.y - 1;
		s.x = 0;
		if (isIn(p,s) != -1)
		    return 1;
		s.x = 2;
		if (isIn(p,s) != -1)
		    return 1;
	    }
	    s.y = lv;
	    if (s.y + 1 <= 2) {
		s.y = s.y + 1;
		s.x = 0;
		if (isIn(p,s) != -1)
		    return 1;
		s.x = 2;
		if (isIn(p,s) != -1)
		    return 1;
	    }
	}
    }
    
    return 0;
}

int isGreen(struct perm4 p) {
    int pv = -1;
    int plane = isPlane(p, &pv);
    if (plane == 0)
	return 0;
    int lv = -1;
    int line = is3Line(p, plane, &lv);
    if (line == 0)
	return 0;
    struct pos s;
    if (plane == 1) {
	s.x = pv;
	if (line == 2) {
	    s.y = lv;
	    if (s.y - 1 >= 0) {
		s.y = s.y - 1;
		s.z = 1;
		if (isIn(p, s) != -1)
		    return 1;
	    }
	    s.y = lv;
	    if (s.y + 1 <= 2) {
		s.y = s.y + 1;
		s.z = 1;
		if (isIn(p, s) != -1)
		    return 1;
	    }
	} else {
	    s.z = lv;
	    if (s.z - 1 >= 0) {
		s.z = s.z - 1;
		s.y = 1;
		if (isIn(p, s) != -1)
		    return 1;
	    }
	    s.z = lv;
	    if (s.z + 1 <= 2) {
		s.z = s.z + 1;
		s.y = 1;
		if (isIn(p,s) != -1)
		    return 1;
	    }
	}
    }
    if (plane == 2) {
	s.y = pv;
	if (line == 1) {
	    s.x = lv;
	    if (s.x -1 >= 0) {
		s.x = s.x - 1;
		s.z = 1;
		if (isIn(p,s) != -1)
		    return 1;
	    }
	    s.x = lv;
	    if (s.x + 1 <= 2) {
		s.x = s.x + 1;
		s.z = 1;
		if (isIn(p,s) != -1)
		    return 1;
	    }
	} else {
	    s.z = lv;
	    if (s.z - 1 >= 0) {
		s.z = s.z - 1;
		s.x = 1;
		if (isIn(p,s) != -1)	
		    return 1;
	    }
	    s.z = lv;
	    if (s.z + 1 <= 2) {
		s.z = s.z + 1;
		s.x = 1;
		if (isIn(p,s) != -1)
		    return 1;
	    }
	}
    }
    if (plane == 3) {
	s.z = pv;
	if (line == 1) {
	    s.x = lv;
	    if (s.x - 1 >= 0) {
		s.x = s.x - 1;
		s.y = 1;
		if (isIn(p,s) != -1)
		    return 1;
	    }
	    s.x = lv;
	    if (s.x + 1 <= 2) {
		s.x = s.x + 1;
		s.y = 1;
		if (isIn(p,s) != -1)
		    return 1;
	    }
	} else {
	    s.y = lv;
	    if (s.y - 1 >= 0) {
		s.y = s.y - 1;
		s.x = 1;
		if (isIn(p,s) != -1)
		    return 1;
	    }
	    s.y = lv;
	    if (s.y + 1 <= 2) {
		s.y = s.y + 1;
		s.x = 1;
		if (isIn(p,s) != -1)
		    return 1;
	    }
	}
    }
    
    return 0;

}

int isOrange(struct perm4 p) {
    int v;
    int plane = isPlane(p, &v);
    if (plane == 0)
	return 0;
    return is22Line(p, plane);
}

int isBlack(struct perm4 p) {
    struct pos s;
    for (int i = 0; i < 4; i++) {
	for (int j = 0; j < 8; j++) {
	    s.x = 2*(j % 2) - 1 + p.p[i].x;
	    s.y = p.p[i].y;
	    s.z = p.p[i].z;
	    if (s.x < 0 || s.x > 2 || isIn(p,s) == -1)
		continue;
	    s.x = p.p[i].x;
	    s.y = 2*(j/2 % 2) - 1 + p.p[i].y;
	    s.z = p.p[i].z;
	    if (s.y < 0 || s.y > 2 || isIn(p,s) == -1)
		continue;
	    s.x = p.p[i].x;
	    s.y = p.p[i].y;
	    s.z =2*(j/4 % 2) - 1 + p.p[i].z;
	    if (s.z < 0 || s.z > 2 || isIn(p,s) == -1)
		continue;
	    return 1;
	}
    }
    return 0;
}

int isBrown(struct perm4 p) {
    struct pos s;
    for (int i = 0; i < 3; i++) {
	for (int j = 0; j < 8; j++) {
	    for (int k = 0; k < 3; k++) {
		if (k != 0) {
		    s.x = 2*(j%2) -1 + p.p[i].x;
		    s.y = p.p[i].y;
		    s.z = p.p[i].z;
		    if (s.x < 0 || s.x > 2 || isIn(p,s) == -1)
			continue;
		}
		if (k != 1) {
		    s.x = p.p[i].x;
		    s.y = 2*(j/2 %2) - 1 + p.p[i].y;
		    s.z = p.p[i].z;
		    if (s.y < 0 || s.y > 2 || isIn(p,s) == -1)
			continue;
		}
		if (k != 2) {
		    s.x = p.p[i].x;
		    s.y = p.p[i].y;
		    s.z = 2*(j/4 %2) - 1 + p.p[i].z;
		    if (s.z < 0 || s.z > 2 || isIn(p,s) == -1)
			continue;
		}
		return 1;
	    }
	}
    }
    return 0;
}

// Returns -1 if not blue or red
// Returns 0 if blue
// Returns 1 if red
int rotParity(struct perm4 p) {
    struct pos s;
    int parity = 0;
    for (int i = 0; i < 4; i++) {
	for (int j = 0; j < 8; j++) {
	    for (int k = 0; k < 3; k++) {
		s.x = p.p[i].x;
		s.y = p.p[i].y;
		s.z = p.p[i].z;
		parity = (j % 2) + (j / 2 % 2) + (j / 4 % 2);
		if (k == 0) {
		    s.x += 1 - 2 * (j % 2);
		    if (s.x < 0 || s.x > 2 || isIn(p,s) == -1)
			continue;
		    s.y += 1 - 2 * (j /2 % 2);
		    if (s.y < 0 || s.y > 2 || isIn(p,s) == -1)
			continue;
		    s.z += 1 - 2 * (j / 4 % 2);
		    if (s.z < 0 || s.z > 2 || isIn(p,s) == -1)
			continue;
		    return parity % 2;
		}
		parity++;
		if (k == 1) {
		    s.x += 1 - 2 * (j % 2);
		    if (s.x < 0 || s.x > 2 || isIn(p,s) == -1)
			continue;
		    s.z += 1 - 2 * (j/4 % 2);
		    if (s.z < 0 || s.z > 2 || isIn(p,s) == -1)
			continue;
		    s.y += 1 - 2 * (j/2 % 2);
		    if (s.y < 0 || s.y > 2 || isIn(p,s) == -1)
			continue;
		    return parity % 2;
		}
		
		if (k == 2) {
		    s.y += 1 - 2 * (j / 2 % 2);
		    if (s.y < 0 || s.y > 2 || isIn(p,s) == -1)
			continue;
		    s.x += 1 - 2 * (j % 2);
		    if (s.x < 0 || s.x > 2 || isIn(p,s) == -1)
			continue;
		    s.z += 1 - 2 * (j / 4 % 2);
		    if (s.z < 0 || s.z > 2 || isIn(p,s) == -1)
			continue;
		    return parity % 2;
		}
	    }
	}
    }
    return -1;
}

int isBlue(struct perm4 p) {
    return rotParity(p) == 0;
}

int isRed(struct perm4 p) {
    return rotParity(p) == 1;
}

int isShape(struct perm4 p, int shape) {
    if (shape == 0)
	return isYellow(p);
    if (shape == 1)
	return isGreen(p);
    if (shape == 2)
	return isOrange(p);
    if (shape == 3)
	return isBlack(p);
    if (shape == 4)
	return isRed(p);
    if (shape == 5)
	return isBlue(p);
}






void run() {
    int validPC;
    int div;
    int codeIndex;
    int i;
    struct perm4 p1;
    struct perm4 p2;
    int kcode;
    int shapeI;
    int shapeCode;
    int shape1;
    int shape2;
    int count;
    //int notValid = 0;
    for (int permCode = 0; permCode < 134217728; permCode++) {
	if (permCode % 1048576 == 0)
	    printf("%F%% of the way there\n", 100 * (permCode / 134217728.0));
	validPC = 0;
	div = 1;
	codeIndex = 0;
	shapeI = 0;
	for (i = 0; i < 27; i++) {
	    validPC += permCode / div % 2;
	    div *= 2;
	}
	if (validPC != 8)
	    continue;
	for (; shapeI < 15; shapeI++) {
	    count = 0;
	    //printf("shape combo %d\n", shapeI);
	    shapeCode = shapes[shapeI];
	    for (i = 0; i < 6; i++) {
		if (shapeCode % 2 == 1) {
		    shape1 = i;
		    break;
		}
		shapeCode /= 2;
	    }
	    i++;
	    shapeCode /= 2;
	    for (; i < 6; i++) {
		if (shapeCode % 2 == 1) {
		    shape2 = i;
		    break;
		}
		shapeCode /= 2;
	    }
	    for (; codeIndex < 70; codeIndex++) {
		kcode = codes[codeIndex];
		assignCode(permCode, kcode, &p1, &p2);
		if (isShape(p1, shape1) && isShape(p2, shape2))
		    count++;
	    }
	    if (count >= 3) {
		printf("K3 found, shapeCodeIndex: %d, permCode: %d\n", shapeI, permCode);
	    }
	}
    }
}

void runBrown() {
    int validPC;
    int div;
    int i;
    struct perm4 p1;
    struct perm4 pB;
    int kcode;
    int shape;
    int count;
    int index = 0;

    
    for (int permCode = 0; permCode < 134217728; permCode++) {
	if (permCode % 1048576 == 0)
	    printf("%F%% of the way there\n", 100 * (permCode / 134217728.0));
	validPC = 0;
	div = 1;
	shape = 0;
	for (i = 0; i < 27; i++) {
	    validPC += permCode / div % 2;
	    div *= 2;
	}
	if (validPC != 7)
	    continue;
	for (; shape < 6; shape++) {
	    count = 0;
	    for (kcode = 0; kcode < 128; kcode++) {
		validPC = 0;
		div = 1;
		for (i = 0; i < 7; i++) {
		    validPC += kcode / div % 2;
		    div *= 2;
		}
		if (validPC != 3)
		    continue;
		assignCodeBrown(permCode, kcode, &p1, &pB);
		if (isShape(p1, shape) && isBrown(pB))
		    count++;
	    }
	    if (count >= 3) {
		index++;
		printf("(%d) K%d found, (not brown) shape: %d, permCode: %d\n", index, count, shape, permCode);
	    }
	}
    }
}


int main(int argc, char** argv) {
    calcCodes();	
    run();
    runBrown();
    return 0;
    struct perm4 p;
    p.p[0].x = 0;
    p.p[0].y = 0;
    p.p[0].z = 0;

    p.p[1].x = 0;
    p.p[1].y = 1;
    p.p[1].z = 0;

    p.p[2].x = 1;
    p.p[2].y = 0;
    p.p[2].z = 0;

    p.p[3].x = 2;
    p.p[3].y = 2;
    p.p[3].z = 2;

    if (isGreen(p))
	printf("dr. green\n");
    if (isOrange(p))
	printf("orang\n");
    if (isYellow(p))
	printf("owyell\n");
    if (isBlack(p))
	printf("black\n");
    if (isBlue(p))
	printf("blue\n");
    if (isRed(p))
	printf("red\n");
    if (isBrown(p))
	printf("brown\n");
}
