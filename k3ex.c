#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>

int codes[70];

int bcodes[35];

int codes3[34650];

int mem[1 << 27];

int shapes[15] = {3, 5, 6, 9, 10, 12, 17, 18, 20, 24, 33, 34, 36, 40, 48};

int shapes3[20] = {8, 9, 10, 11, 15, 16, 17, 22, 23, 29, 51, 52, 53, 58, 59, 65, 94, 95, 101, 137};

struct pos {
    int x;
    int y;
    int z;
};

struct perm {
    struct pos p[8];
};

struct perm4 { 
    int permCode;
    int diff;
    struct pos p[4];
};

int graph[27][7];

void calcGraph() {
    int j;
    for (int i = 0; i < 27; i++) {
	j = 0;
	if (i % 3 != 0) {
	    graph[i][j] = i - 1;
	    j++;
	}
	if (i % 3 != 2) {
	    graph[i][j] = i + 1;
	    j++;
	}
	if (i / 3 % 3 != 0) {
	    graph[i][j] = i - 3;
	    j++;
	}
	if (i / 3 % 3 != 2) {
	    graph[i][j] = i + 3;
	    j++;
	}
	if (i / 9 % 3 != 0) {
	    graph[i][j] = i - 9;
	    j++;
	}
	if (i / 9 % 3 != 2) {
	    graph[i][j] = i + 9;
	    j++;
	}
	graph[i][j] = -1;
    }
}

int connected(int permcode, int size) {
    int opc = permcode;
    if (size == 4 && mem[opc] != 0) {
	return mem[opc] == 1;
    }
    int has[27];
    int found[27]; // size
    int j = 0;
    for (int i = 0; i < 27; i++) {
	has[i] = permcode % 2;
	if (permcode % 2 == 1 && j == 0) {
	    found[j] = i;
	    has[i] = 0;
	    j++;
	}
	permcode /= 2;
    }
    for (int i = 0; i < j; i++) {
	for (int k = 0; graph[found[i]][k] != -1; k++) {
	    if (has[graph[found[i]][k]]) {
		has[graph[found[i]][k]] = 0;
		found[j] = graph[found[i]][k];
		j++;
	    }
	    if (j > size - 1) {
		if (size == 4)
		    mem[opc] = 1;
		return 1;
	    }
	}
    }
    if (size == 4) {
	mem[opc] = -1;
    }
    return 0;
}


void calcCodes() {
    int i = 0;
    for (int code = 0; code < 256; code++) {
	if ( (code % 2) + (code/2 % 2) + (code/4 % 2) + (code/8 % 2) + (code/16 % 2) + (code/32 % 2) + (code/64 % 2) + (code/128 % 2) == 4 ) {
	   codes[i] = code;
	   i++;
	}
    }
    i = 0;
    for (int code = 0; code < 128; code++) {
	if ( (code %2) + (code/2 % 2) + (code/4 % 2) + (code/8 % 2) + (code/16 %2) + (code/32 %2) + (code/64 %2) == 3) {
	    bcodes[i] = code;
	    i++;
	}
    }
    i = 0;
    int arr[3];
    int div;
    for (int code = 0; code < 531441; code++) {
	arr[0] = 0;
	arr[1] = 0;
	arr[2] = 0;
	div = 1;
	for (int j = 0; j < 12; j++) {
	    arr[code / div % 3] += 1;
	    div *= 3;
	}
	if (arr[0] == 4 && arr[1] == 4 && arr[2] == 4) {
	    codes3[i] = code;
	    i++;
	}
    }
    int count = 0;
    for (int z = 0; z < 1 << 27; z++) {
	count = 0;
	for (int i = 0; i < 27; i++) {
	    if (z / (1 << i) % 2 == 1)
		count++;
	}
	if (count != 4)
	    continue;
	mem[z] = 0;
	connected(z, 4);
    }
}



int assignCode(int permCode, int kcode, struct perm4 *p1, struct perm4 *p2) {
    p1->diff = 0;
    p2->diff = 0;
    p1->permCode = 0;
    p2->permCode = 0;
    int i1 = 0;
    int i2 = 0;
    for (int i = 0; i < 27; i++) {
	if (permCode % 2) {
	    if (kcode % 2) {
		p2->permCode += 1 << i;
		p2->p[i2].x = i % 3;
		p2->p[i2].y = i / 3 % 3;
		p2->p[i2].z = i / 9 % 3;
		p2->diff += 2*((i % 3 + i / 3 % 3 + i / 9 % 3) % 2) - 1;
		i2++;
	    } else {
		p1->permCode += 1 << i;
		p1->p[i1].x = i % 3;
		p1->p[i1].y = i / 3 % 3;
		p1->p[i1].z = i / 9 % 3;
		p1->diff += 2*((i % 3 + i / 3 % 3 + i / 9 % 3) % 2) - 1;
		i1++;
	    }
	    kcode /= 2;
	}
	permCode /= 2;
    }
    if (p1->diff < 0)
	p1->diff *= -1;
    if (p2->diff < 0)
	p2->diff *= -1;
    return !((i1 == 4) && (i2 == 4));
}

int assignCode3(int permCode, int kcode, struct perm4 *p1, struct perm4 *p2, struct perm4 *p3) {
    p1->diff = 0;
    p2->diff = 0;
    p3->diff = 0;
    p1->permCode = 0;
    p2->permCode = 0;
    p3->permCode = 0;
    int i1 = 0;
    int i2 = 0;
    int i3 = 0;
    for (int i = 0; i < 27; i++) {
	if (permCode % 2) {
	    if (kcode % 3 == 2) {
		p3->permCode += 1 << i;
		p3->p[i3].x = i % 3;
		p3->p[i3].y = i / 3 % 3;
		p3->p[i3].z = i / 9 % 3;
		p3->diff += 2*((i % 3 + i / 3 % 3 + i / 9 % 3) % 2) - 1;
		i3++;
	    }
	    if (kcode % 3 == 1) {
		p2->permCode += 1 << i;
		p2->p[i2].x = i % 3;
		p2->p[i2].y = i / 3 % 3;
		p2->p[i2].z = i / 9 % 3;
		p2->diff += 2*((i % 3 + i / 3 % 3 + i / 9 % 3) % 2) - 1;
		i2++;
	    }
	    if (kcode % 3 == 0) {
		p1->permCode += 1 << i;
		p1->p[i1].x = i % 3;
		p1->p[i1].y = i / 3 % 3;
		p1->p[i1].z = i / 9 % 3;
		p1->diff += 2*((i % 3 + i / 3 % 3 + i / 9 % 3) % 2) - 1;
		i1++;
	    }
	    kcode /= 3;
	}
	permCode /= 2;
    }
    if (p1->diff < 0)
	p1->diff *= -1;
    if (p2->diff < 0)
	p2->diff *= -1;
    if (p3->diff < 0)
	p3->diff *= -1;
}

int assignCodeBrown(int permCode, int kcode, struct perm4 *p1, struct perm4 *pB) {
    int i1 = 0;
    int i2 = 0;
    p1->diff = 0;
    pB->diff = 0;
    p1->permCode = 0;
    pB->permCode = 0;
    for (int i = 0; i < 27; i++) {
	if (permCode % 2) {
	    if (kcode % 2) {
		pB->permCode += 1 << i;
		pB->p[i2].x = i % 3;
		pB->p[i2].y = i / 3 % 3;
		pB->p[i2].z = i / 9 % 3;
		pB->diff += 2*((i % 3 + i / 3 % 3 + i / 9 % 3) % 2) - 1;
		i2++;
	    } else {
		p1->permCode += 1 << i;
		p1->p[i1].x = i % 3;
		p1->p[i1].y = i / 3 % 3;
		p1->p[i1].z = i / 9 % 3;
		p1->diff += 2*((i % 3 + i / 3 % 3 + i / 9 % 3) % 2) - 1;
		i1++;
	    }
	    kcode /= 2;
	}
	permCode /= 2;
    }
    if (p1->diff < 0)
	p1->diff *= -1;
    if (pB->diff < 0)
	pB->diff *= -1;
    pB->p[3].x = pB->p[0].x;
    pB->p[3].y = pB->p[0].y;
    pB->p[3].z = pB->p[0].z;
    return (i2 == 3) && (i1 == 4);
}

// Returns 0 on false
// Returns 1 for x-plane
// Returns 2 for y-plane
// Returns 3 for z-plane
int isPlane(struct perm4 *p, int *v) {
    if (p->p[0].x == p->p[1].x && p->p[1].x == p->p[2].x && p->p[2].x == p->p[3].x) {
	*v = p->p[0].x;
	return 1;
    }
    if (p->p[0].y == p->p[1].y && p->p[1].y == p->p[2].y && p->p[2].y == p->p[3].y) {
	*v = p->p[0].y;
	return 2;
    }
    if (p->p[0].z == p->p[1].z && p->p[1].z == p->p[2].z && p->p[2].z == p->p[3].z) {
	*v = p->p[0].z;
	return 3;
    }
    return 0;
}

// Returns 0 on false
// Returns 1 on x-line
// Returns 2 on y-line
// Returns 3 on z-line
// Never returns plane (i.e., the nonzero return from isPlane should be put in plane)
int is3Line(struct perm4 *p, int plane, int *v) {
    int x[3] = {0, 0 ,0};
    int y[3] = {0, 0, 0};
    int z[3] = {0, 0, 0};
    if (plane != 1) {
	for (int i = 0; i < 4; i++) {
	    x[p->p[i].x]++;
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
	    y[p->p[i].y]++;
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
	    z[p->p[i].z]++;
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
int is22Line(struct perm4 *p, int plane) {
    int a[3] = {0, 0, 0};
    int b[3] = {0, 0, 0};
    if (plane == 1) { // a=y, b=z
	for (int i = 0; i < 4; i++) {
	    a[p->p[i].y]++;
	    b[p->p[i].z]++;
	}
    } 
    if (plane == 2) { // a=x, b=z
	for (int i = 0; i < 4; i++) {
	    a[p->p[i].x]++;
	    b[p->p[i].z]++;
	}
    } 
    if (plane == 3) { // a=x, b=y
	for (int i = 0; i < 4; i++) {
	    a[p->p[i].x]++;
	    b[p->p[i].y]++;
	}
    }
    return (a[1] == b[1]) && (b[1] == 2) && ((a[0] == 1) ^ (b[0] == 1));
}


// Returns -1 on false
// Returns pos index on match
int isIn(struct perm4 *p, struct pos *s) {
    for (int i = 0; i < 4; i++) {
	if (p->p[i].x == s->x && p->p[i].y == s->y && p->p[i].z == s->z)
	    return i;
    }
    return -1;
}


// All piece checks
// Return 0 on false
// Return 1 on true

int isYellow(struct perm4 *p) {
    if (p->diff != 0)
	return 0;
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
		if (isIn(p, &s) != -1)
		    return 1;
		s.z = 2;
		if (isIn(p, &s) != -1)
		    return 1;
	    }
	    s.y = lv;
	    if (s.y + 1 <= 2) {
		s.y = s.y + 1;
		s.z = 0;
		if (isIn(p, &s) != -1)
		    return 1;
		s.z = 2;
		if (isIn(p, &s) != -1)
		    return 1;
	    }
	} else {
	    s.z = lv;
	    if (s.z - 1 >= 0) {
		s.z = s.z - 1;
		s.y = 0;
		if (isIn(p, &s) != -1)
		    return 1;
		s.y = 2;
		if (isIn(p,&s) != -1)
		    return 1;
	    }
	    s.z = lv;
	    if (s.z + 1 <= 2) {
		s.z = s.z + 1;
		s.y = 0;
		if (isIn(p,&s) != -1)
		    return 1;
		s.y = 2;
		if (isIn(p,&s) != -1)
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
		if (isIn(p,&s) != -1)
		    return 1;
		s.z = 2;
		if (isIn(p, &s) != -1)
		    return 1;
	    }
	    s.x = lv;
	    if (s.x + 1 <= 2) {
		s.x = s.x + 1;
		s.z = 0;
		if (isIn(p,&s) != -1)
		    return 1;
		s.z = 2;
		if (isIn(p,&s) != -1)
		    return 1;
	    }
	} else {
	    s.z = lv;
	    if (s.z - 1 >= 0) {
		s.z = s.z - 1;
		s.x = 0;
		if (isIn(p,&s) != -1)	
		    return 1;
		s.x = 2;
		if (isIn(p,&s) != -1)
		    return 1;
	    }
	    s.z = lv;
	    if (s.z + 1 <= 2) {
		s.z = s.z + 1;
		s.x = 0;
		if (isIn(p,&s) != -1)
		    return 1;
		s.x = 2;
		if (isIn(p,&s) != -1)
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
		if (isIn(p,&s) != -1)
		    return 1;
		s.y = 2;
		if (isIn(p,&s) != -1)
		    return 1;
	    }
	    s.x = lv;
	    if (s.x + 1 <= 2) {
		s.x = s.x + 1;
		s.y = 0;
		if (isIn(p,&s) != -1)
		    return 1;
		s.y = 2;
		if (isIn(p,&s) != -1)
		    return 1;
	    }
	} else {
	    s.y = lv;
	    if (s.y - 1 >= 0) {
		s.y = s.y - 1;
		s.x = 0;
		if (isIn(p,&s) != -1)
		    return 1;
		s.x = 2;
		if (isIn(p,&s) != -1)
		    return 1;
	    }
	    s.y = lv;
	    if (s.y + 1 <= 2) {
		s.y = s.y + 1;
		s.x = 0;
		if (isIn(p,&s) != -1)
		    return 1;
		s.x = 2;
		if (isIn(p,&s) != -1)
		    return 1;
	    }
	}
    }
    
    return 0;
}

int isGreen(struct perm4 *p) {
    if (p->diff != 2)
	return 0;
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
		if (isIn(p, &s) != -1)
		    return 1;
	    }
	    s.y = lv;
	    if (s.y + 1 <= 2) {
		s.y = s.y + 1;
		s.z = 1;
		if (isIn(p, &s) != -1)
		    return 1;
	    }
	} else {
	    s.z = lv;
	    if (s.z - 1 >= 0) {
		s.z = s.z - 1;
		s.y = 1;
		if (isIn(p, &s) != -1)
		    return 1;
	    }
	    s.z = lv;
	    if (s.z + 1 <= 2) {
		s.z = s.z + 1;
		s.y = 1;
		if (isIn(p,&s) != -1)
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
		if (isIn(p,&s) != -1)
		    return 1;
	    }
	    s.x = lv;
	    if (s.x + 1 <= 2) {
		s.x = s.x + 1;
		s.z = 1;
		if (isIn(p,&s) != -1)
		    return 1;
	    }
	} else {
	    s.z = lv;
	    if (s.z - 1 >= 0) {
		s.z = s.z - 1;
		s.x = 1;
		if (isIn(p,&s) != -1)	
		    return 1;
	    }
	    s.z = lv;
	    if (s.z + 1 <= 2) {
		s.z = s.z + 1;
		s.x = 1;
		if (isIn(p,&s) != -1)
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
		if (isIn(p,&s) != -1)
		    return 1;
	    }
	    s.x = lv;
	    if (s.x + 1 <= 2) {
		s.x = s.x + 1;
		s.y = 1;
		if (isIn(p,&s) != -1)
		    return 1;
	    }
	} else {
	    s.y = lv;
	    if (s.y - 1 >= 0) {
		s.y = s.y - 1;
		s.x = 1;
		if (isIn(p,&s) != -1)
		    return 1;
	    }
	    s.y = lv;
	    if (s.y + 1 <= 2) {
		s.y = s.y + 1;
		s.x = 1;
		if (isIn(p,&s) != -1)
		    return 1;
	    }
	}
    }
    
    return 0;

}

int isOrange(struct perm4 *p) {
    if (p->diff != 0)
	return 0;
    int v;
    int plane = isPlane(p, &v);
    if (plane == 0)
	return 0;
    return is22Line(p, plane);
}

int isBlack(struct perm4 *p) {
    if (p->diff != 2)
	return 0;
    struct pos s;
    for (int i = 0; i < 4; i++) {
	for (int j = 0; j < 8; j++) {
	    s.x = 2*(j % 2) - 1 + p->p[i].x;
	    s.y = p->p[i].y;
	    s.z = p->p[i].z;
	    if (s.x < 0 || s.x > 2 || isIn(p,&s) == -1)
		continue;
	    s.x = p->p[i].x;
	    s.y = 2*(j/2 % 2) - 1 + p->p[i].y;
	    s.z = p->p[i].z;
	    if (s.y < 0 || s.y > 2 || isIn(p,&s) == -1)
		continue;
	    s.x = p->p[i].x;
	    s.y = p->p[i].y;
	    s.z =2*(j/4 % 2) - 1 + p->p[i].z;
	    if (s.z < 0 || s.z > 2 || isIn(p,&s) == -1)
		continue;
	    return 1;
	}
    }
    return 0;
}

int isBrown(struct perm4 *p) {
    if (p->diff != 1)
	return 0;
    if (!connected(p->permCode, 3))
	return 0;
    struct pos s;
    for (int i = 0; i < 3; i++) {
	for (int j = 0; j < 8; j++) {
	    for (int k = 0; k < 3; k++) {
		if (k != 0) {
		    s.x = 2*(j%2) -1 + p->p[i].x;
		    s.y = p->p[i].y;
		    s.z = p->p[i].z;
		    if (s.x < 0 || s.x > 2 || isIn(p,&s) == -1)
			continue;
		}
		if (k != 1) {
		    s.x = p->p[i].x;
		    s.y = 2*(j/2 %2) - 1 + p->p[i].y;
		    s.z = p->p[i].z;
		    if (s.y < 0 || s.y > 2 || isIn(p,&s) == -1)
			continue;
		}
		if (k != 2) {
		    s.x = p->p[i].x;
		    s.y = p->p[i].y;
		    s.z = 2*(j/4 %2) - 1 + p->p[i].z;
		    if (s.z < 0 || s.z > 2 || isIn(p,&s) == -1)
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
int rotParity(struct perm4 *p) {
    if (p->diff != 0)
	return -1;
    struct pos s;
    int parity = 0;
    for (int i = 0; i < 4; i++) {
	for (int j = 0; j < 8; j++) {
	    for (int k = 0; k < 3; k++) {
		s.x = p->p[i].x;
		s.y = p->p[i].y;
		s.z = p->p[i].z;
		parity = (j % 2) + (j / 2 % 2) + (j / 4 % 2);
		if (k == 0) {
		    s.x += 1 - 2 * (j % 2);
		    if (s.x < 0 || s.x > 2 || isIn(p,&s) == -1)
			continue;
		    s.y += 1 - 2 * (j /2 % 2);
		    if (s.y < 0 || s.y > 2 || isIn(p,&s) == -1)
			continue;
		    s.z += 1 - 2 * (j / 4 % 2);
		    if (s.z < 0 || s.z > 2 || isIn(p,&s) == -1)
			continue;
		    return parity % 2;
		}
		parity++;
		if (k == 1) {
		    s.x += 1 - 2 * (j % 2);
		    if (s.x < 0 || s.x > 2 || isIn(p,&s) == -1)
			continue;
		    s.z += 1 - 2 * (j/4 % 2);
		    if (s.z < 0 || s.z > 2 || isIn(p,&s) == -1)
			continue;
		    s.y += 1 - 2 * (j/2 % 2);
		    if (s.y < 0 || s.y > 2 || isIn(p,&s) == -1)
			continue;
		    return parity % 2;
		}
		
		if (k == 2) {
		    s.y += 1 - 2 * (j / 2 % 2);
		    if (s.y < 0 || s.y > 2 || isIn(p,&s) == -1)
			continue;
		    s.x += 1 - 2 * (j % 2);
		    if (s.x < 0 || s.x > 2 || isIn(p,&s) == -1)
			continue;
		    s.z += 1 - 2 * (j / 4 % 2);
		    if (s.z < 0 || s.z > 2 || isIn(p,&s) == -1)
			continue;
		    return parity % 2;
		}
	    }
	}
    }
    return -1;
}

int isBlue(struct perm4 *p) {
    return rotParity(p) == 0;
}

int isRed(struct perm4 *p) {
    return rotParity(p) == 1;
}

int isShape(struct perm4 *p, int shape) {
    //if (!connected(p->permCode, 4))
//	return 0;
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


void runConnected() {
    int numConnected = 0;
    int numSubGraphs = 0;
    int validPC;
    int div;
    int testSize = 3;
    for (int permCode = 0; permCode < 134217728; permCode++) {
	validPC = 0;
	div = 1;
	for (int i = 0; i < 27; i++) {
	    validPC += permCode / div % 2;
	    div *= 2;
	}
	if (validPC != testSize)
	    continue;
	numSubGraphs++;
	if (connected(permCode, testSize)) {
	    numConnected++;
	}
    }
    printf("Number of Subgraphs: %d\n", numSubGraphs);
    printf("Number connected: %d\n", numConnected);
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
	//if (permCode % 1048576 == 0)
	    //printf("%F%% of the way there\n", 100 * (permCode / 134217728.0));
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
	if (!connected(permCode, 8))
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
	    for (codeIndex = 0; codeIndex < 70; codeIndex++) {
		kcode = codes[codeIndex];
		assignCode(permCode, kcode, &p1, &p2);
		if (isShape(&p1, shape1) && isShape(&p2, shape2))
		    count++;
	    }
	    if (count >= 3) {
		printf("K3 found, shapeCodeIndex: %d, permCode: %d\n", shapeI, permCode);
	    }
	}
    }
}

void run3() {
    int validPC;
    int div;
    int codeIndex;
    int i;
    struct perm4 p1;
    struct perm4 p2;
    struct perm4 p3;
    int kcode;
    int shapeI;
    int shapeCode;
    int shape1;
    int shape2;
    int shape3;
    int count[20];
    int present[20];
    for (shapeI = 0; shapeI < 20; shapeI++) {
	present[shapeI] = 0;
    }
    //int notValid = 0;
    for (int permCode = 0; permCode < 134217728; permCode++) {
	if (permCode % 131072 == 0)
	    printf("%F%% of the way there\n", 100 * (permCode / 134217728.0));
	validPC = 0;
	div = 1;
	for (i = 0; i < 27; i++) {
	    validPC += permCode / div % 2;
	    div *= 2;
	}
	if (validPC != 12)
	    continue;
	if (!connected(permCode, 12))
	    continue;
	for (shapeI = 0; shapeI < 20; shapeI++) {
	    count[shapeI] = 0;
	}
	for (codeIndex = 0; codeIndex < 34650; codeIndex++) {
	    kcode = codes3[codeIndex];
	    assignCode3(permCode, kcode, &p1, &p2, &p3);
	    if (!connected(p1.permCode, 4) || !connected(p2.permCode, 4) || !connected(p3.permCode, 4)) {
		continue;
	    }
	    for (shapeI = 0; shapeI < 20; shapeI++) {
		shapeCode = shapes3[shapeI];
		shape3 = shapeCode % 6;
		shape2 = shapeCode / 6 % 6;
		shape1 = shapeCode / 36 % 6;
		if (isShape(&p1, shape1) && isShape(&p2, shape2) && isShape(&p3, shape3))
		    count[shapeI]++;
	    }
	}

	for (shapeI = 0; shapeI < 20; shapeI++) {
	    if (count[shapeI] >= 3) {
		printf("K%d found, shapeCodeIndex: %d, permCode: %d\n", count[shapeI], shapeI, permCode);
	    }
	}
    }
}



/*void* Trun3(void *arg) {
    int validPC;
    int div;
    int codeIndex;
    int i;
    struct perm4 p1;
    struct perm4 p2;
    struct perm4 p3;
    int kcode;
    int shapeI = *( (int *) arg);
    int shapeCode = shapes3[shapeI];
    int shape1 = shapeCode / 36 % 6;
    int shape2 = shapeCode / 6 % 6;
    int shape3 = shapeCode % 6;
    int count = 0;
    int present = 0;
    //int notValid = 0;
    for (int permCode = 0; permCode < 134217728; permCode++) {
	if (permCode % 65536 == 0 && shapeI == 0)
	    printf("Thread %d: %F%% of the way there\n", shapeI, 100 * (permCode / 134217728.0));
	validPC = 0;
	div = 1;
	for (i = 0; i < 27; i++) {
	    validPC += permCode / div % 2;
	    div *= 2;
	}
	if (validPC != 12)
	    continue;
	if (!connected(permCode, 12))
	    continue;
	count = 0;
	for (codeIndex = 0; codeIndex < 34650; codeIndex++) {
	    kcode = codes3[codeIndex];
	    assignCode3(permCode, kcode, &p1, &p2, &p3);
	    if (!connected(p1.permCode, 4) || !connected(p2.permCode, 4) || !connected(p3.permCode, 4)) {
		continue;
	    }
	    if (isShape(&p1, shape1) && isShape(&p2, shape2) && isShape(&p3, shape3)) {
		count++;
		present++;
	    }
	}
	if (count >= 3) {
	    printf("K%d found, shapeCodeIndex: %d, permCode: %d\n", count, shapeI, permCode);
	}
    }
    printf("shapeCodeIndex: %d, number of 3-swaps: %d\n", shapeI, present);
    return NULL;
}*/

void* Trun3(void* arg) {
    int validPC;
    int div;
    int codeIndex;
    int i;
    struct perm4 p1;
    struct perm4 p2;
    struct perm4 p3;
    int kcode;
    int shapeI;
    int shapeCode;
    int shape1;
    int shape2;
    int shape3;
    int count[20];
    int present[20];
    int start = *((int *) arg);
    for (shapeI = 0; shapeI < 20; shapeI++) {
	present[shapeI] = 0;
    }
    //int notValid = 0;
    for (int permCode = start; permCode < 1 << 27; permCode += 16) {
	if (permCode % (1 << 20) == start)
	    printf("Thread %d: %F%% of the way there\n", start, 100 * (permCode / 134217728.0));
	validPC = 0;
	div = 1;
	for (i = 0; i < 27; i++) {
	    validPC += permCode / div % 2;
	    div *= 2;
	}
	if (validPC != 12)
	    continue;
	if (!connected(permCode, 12))
	    continue;
	for (shapeI = 0; shapeI < 20; shapeI++) {
	    count[shapeI] = 0;
	}
	for (codeIndex = 0; codeIndex < 34650; codeIndex++) {
	    kcode = codes3[codeIndex];
	    assignCode3(permCode, kcode, &p1, &p2, &p3);
	    if (!connected(p1.permCode, 4) || !connected(p2.permCode, 4) || !connected(p3.permCode, 4)) {
		continue;
	    }
	    for (shapeI = 0; shapeI < 20; shapeI++) {
		shapeCode = shapes3[shapeI];
		shape3 = shapeCode % 6;
		shape2 = shapeCode / 6 % 6;
		shape1 = shapeCode / 36 % 6;
		if (isShape(&p1, shape1) && isShape(&p2, shape2) && isShape(&p3, shape3)) {
		    count[shapeI]++;
		}
	    }
	}

	for (shapeI = 0; shapeI < 20; shapeI++) {
	    if (count[shapeI] > present[shapeI])
		present[shapeI] = count[shapeI];
	}
    }
    printf("\nThread %d Done\n\n", start);
    for (shapeI = 0; shapeI < 20; shapeI++) {
	printf("\nThread %d: Largest shapeI %d: %d\n\n", start, shapeI, present[shapeI]);
    }
}

void runBrown() {
    int validPC;
    int div;
    int i;
    struct perm4 p1;
    struct perm4 pB;
    int kcode;
    int cind;
    int shape;
    int count;
    int index = 0;

    
    for (int permCode = 0; permCode < 134217728; permCode++) {
	//if (permCode % 1048576 == 0)
	    //printf("%F%% of the way there\n", 100 * (permCode / 134217728.0));
	validPC = 0;
	div = 1;
	shape = 0;
	for (i = 0; i < 27; i++) {
	    validPC += permCode / div % 2;
	    div *= 2;
	}
	if (validPC != 7)
	    continue;
	if (!connected(permCode, 7))
	    continue;
	for (; shape < 6; shape++) {
	    count = 0;
	    for (cind = 0; cind < 35; cind++) {
		kcode = bcodes[cind];
		assignCodeBrown(permCode, kcode, &p1, &pB);
		if (isShape(&p1, shape) && isBrown(&pB))
		    count++;
	    }
	    if (count >= 3) {
		index++;
		//printf("(%d) K%d found, (not brown) shape: %d, permCode: %d\n", index, count, shape, permCode);
	    }
	}
    }
    printf("K3's found: %d\n", index);
}


int main(int argc, char** argv) {
    calcGraph();
    //runConnected();
    //return 0;
    calcCodes();
    struct timeval startt, stopt;
    pthread_t threads[16];
    int arg[16];
    for (int i = 0; i < 16; i++) {
	arg[i] = i;
    }
    printf("starting\n");
    gettimeofday(&startt, NULL);
    //run();
    for (int i = 0; i < 16; i++) {
	pthread_create(&threads[i], NULL, Trun3, &arg[i]);
    }
    for (int i = 0; i < 16; i++) {
	pthread_join(threads[i], NULL);
    }
    //run3();
    gettimeofday(&stopt, NULL);
    printf("s (w/o Brown): %ld\n", stopt.tv_sec - startt.tv_sec);
    return 0;
    gettimeofday(&startt, NULL);
    runBrown();
    gettimeofday(&stopt, NULL);
    printf("s (w/ Brown): %ld\n", stopt.tv_sec - startt.tv_sec);
    return 0;
    /*
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
	*/
}
