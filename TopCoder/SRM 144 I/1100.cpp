#include <cstdlib>
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

#define MIN(x,y) ((x)<(y) ? (x) : (y))
#define MAX(x,y) ((x)>(y) ? (x) : (y))

class point {
public:
	int x;
	int y;
	point() : x(200), y(200) {}
	point(const int _x, const int _y) : x(_x), y(_y) {}
	int operator[](const int idx) const { return idx ? y : x; }
	int & operator()(const int idx) { return idx ? y : x; }
	bool operator<(const point & rhs) const {
		if(x != rhs.x)
			return x < rhs.x;
		return y < rhs.y;
	}
};

class segment;

class intersection {
public:
	intersection(const point _p, segment * _h = NULL, segment * _v = NULL) : p(_p), visited(false), h(_h), v(_v), l(NULL), r(NULL), u(NULL), d(NULL) {}
	point p;
	bool visited;
	segment * h, * v;
	bool operator<(const intersection & rhs) const { return p < rhs.p; }
	intersection * l, * r, * u, * d;
}; 

struct pointerIntersectLessThan {
	bool operator() (const intersection * lhs, const intersection * rhs) const {return *lhs < *rhs;}
};

class segment {
public: // have to pay attention when adding so that o < d
	segment() : o(350,350), d(350,350) {}
	segment(const point _o, const point _d) : o(_o), d(_d) {
		if( !(o < d) ) {
			point swap = o;
			o = d;
			d = swap;
		}
	}
	point o;
	point d;
	set<point> intersects;
	bool vertical() const { return o.x == d.x; }
	bool horizontal() const { return o.y == d.y; }
	int orientation() const { return horizontal(); }
	int mainAxis() const { return o[orientation()]; }
	int beginSec() const { return o[!orientation()]; }
	int endSec() const { return d[!orientation()]; }
	bool operator<(const segment & rhs) const { return beginSec() < rhs.beginSec(); }
	void join(segment tj) {
		bool ori = !orientation();
		//cout << "joining P(" << o.x << "," << o.y << ")--P(" << d.x << "," << d.y << ") with P("
		//     << tj.o.x << "," << tj.o.y << ")--P(" << tj.d.x  << "," << tj.d.y << ")" << endl;
		o(ori) = MIN(o[ori],tj.o[ori]);
		d(ori) = MAX(d[ori],tj.d[ori]);
		//cout << "result is P(" << o.x << "," << o.y << ")--P(" << d.x << "," << d.y << ")" << endl;
	}
};

class segmentSet {
public:
	segmentSet(bool _ori) : ori(_ori) {}
	map< int, set<segment> > m;
	const int ori;
	void addSegSameOrientation(segment s) {
		if(!m.count(s.o[ori])) {
			//cout << "here with s=(" << s.o.x << "," << s.o.y << ")--(" << s.d.x << "," << s.d.y << ")" << endl;
			m[s.o[ori]].insert(s);
		} else {
			set<segment>::iterator it;
			do {
				it = m[s.o[ori]].lower_bound(s);
				if(it != m[s.o[ori]].begin()) {
					--it;
					if(it->d[!ori] >= s.o[!ori]) {
						s.join(*it);
						m[s.o[ori]].erase(it);
						continue;
					}
				}
				break;
			} while(1);
			do {
				it = m[s.o[ori]].lower_bound(s);
				if(it != m[s.o[ori]].end()) {
					if(it->o[!ori] <= s.d[!ori]) {
						s.join(*it);
						m[s.o[ori]].erase(it);
						continue;
					}
				}
				break;
			} while(1);
			m[s.o[ori]].insert(s);
		}
	}
};

class PenLift {
public:
	PenLift() : ver(0), hor(1) {}
	bool odd;
	segmentSet ver;
	segmentSet hor;
	set<intersection> itrscts;
	void addIntersection(point p, segment * h, segment * v) {
		//cout << "found an intersection on P(" << p.x << "," << p.y << ")" << endl;
		intersection i(p, h, v);
		itrscts.insert(i);
		if(h!=NULL) h->intersects.insert(p);
		if(v!=NULL) v->intersects.insert(p);
	}
	void addExtremity(point p, segment * h, segment * v) {
		intersection i(p, h, v);
		if(!itrscts.count(i)) {
			//cout << "adding an extremity on P(" << p.x << "," << p.y << ")" << endl;
			itrscts.insert(i);
			if(h!=NULL) h->intersects.insert(p);
			if(v!=NULL) v->intersects.insert(p);
		}
	}
	intersection * find_l(intersection * i) {
		if(i->h == NULL)
			return NULL;
		set<point>::iterator p = i->h->intersects.find(i->p);
		if(p != i->h->intersects.begin()) {
			//cout << "the left neighbor of P(" << p->x << "," << p->y << ")" << endl;
			--p;
			//cout << "is P(" << p->x << "," << p->y << ")" << endl;
			intersection toFind(*p);
			set<intersection>::iterator it = itrscts.find(toFind);
			return const_cast<intersection *> (&(*it));
		}
		return NULL;
	}
	intersection * find_r(intersection * i) {
		if(i->h == NULL)
			return NULL;
		set<point>::iterator p = i->h->intersects.find(i->p);
		//cout << "the right neighbor of P(" << p->x << "," << p->y << ")" << endl;
		if(++p != i->h->intersects.end()) {
			//cout << "is P(" << p->x << "," << p->y << ")" << endl;
			intersection toFind(*p);
			set<intersection>::iterator it = itrscts.find(toFind);
			return const_cast<intersection *> (&(*it));
		}
		return NULL;
	}
	intersection * find_d(intersection * i) {
		if(i->v == NULL)
			return NULL;
		set<point>::iterator p = i->v->intersects.find(i->p);
		if(p != i->v->intersects.begin()) {
			//cout << "the down neighbor of P(" << p->x << "," << p->y << ")" << endl;
			--p;
			//cout << "is P(" << p->x << "," << p->y << ")" << endl;
			intersection toFind(*p);
			set<intersection>::iterator it = itrscts.find(toFind);
			return const_cast<intersection *> (&(*it));
		}
		return NULL;
	}
	intersection * find_u(intersection * i) {
		if(i->v == NULL)
			return NULL;
		set<point>::iterator p = i->v->intersects.find(i->p);
		//cout << "the up neighbor of P(" << p->x << "," << p->y << ")" << endl;
		if(++p != i->v->intersects.end()) {
			//cout << "is P(" << p->x << "," << p->y << ")" << endl;
			intersection toFind(*p);
			set<intersection>::iterator it = itrscts.find(toFind);
			return const_cast<intersection *> (&(*it));
		}
		return NULL;
	}
	int visit(const intersection * ci) {
		intersection * i = const_cast<intersection *> (ci);
		if(i->visited)
			return 0;
		i->visited = true;
		int ngbrs = 0;
		int total = 0;
		if(ci->l != NULL) {
			++ngbrs;
			total += visit(ci->l);
		}
		if(ci->r != NULL) {
			++ngbrs;
			total += visit(ci->r);
		}
		if(ci->d != NULL) {
			++ngbrs;
			total += visit(ci->d);
		}
		if(ci->u != NULL) {
			++ngbrs;
			total += visit(ci->u);
		}
		//cout << "visit P(" << ci->p.x << "," << ci->p.y << ") results in " << ((ngbrs%2) && odd) << ", and total is currently " << total << endl;
		return ((ngbrs%2) && odd) + total;
	}
	int numTimes(vector<string> segments, int n);
};

int PenLift::numTimes(vector<string> segments, int n) {
	odd = n%2;	
	for(int i = 0 ; i < segments.size() ; ++i) {
		stringstream ss (stringstream::in | stringstream::out);
		ss << segments[i];
		int ox, oy, dx, dy;
		ss >> ox >> oy >> dx >> dy;
		segment s(point(ox,oy),point(dx,dy));
		if(s.vertical()) {
			//cout << "adding a vertical" << endl;
			ver.addSegSameOrientation(s);
		} else {
			//cout << "adding a horizontal" << endl;
			hor.addSegSameOrientation(s);
		}
	}
	for(map< int, set<segment> >::iterator vi = ver.m.begin() ; vi != ver.m.end() ; ++vi) {
		set<segment>::iterator vj;
		for(vj = vi->second.begin() ; vj != vi->second.end() ; ++vj) {
			map< int, set<segment> >::iterator hi;
			for(hi = hor.m.lower_bound(vj->o.y) ; hi != hor.m.upper_bound(vj->d.y) ; ++hi) {
				set<segment>::iterator hj;
				hj = hi->second.upper_bound(segment(point(vj->o.x,hi->first),point(vj->o.x,hi->first)));
				if(hj != hi->second.begin()) {
					--hj;
					if(hj->d.x >= vj->o.x)
						addIntersection(point(vj->o.x,hj->d.y), const_cast<segment *>(&(*hj)), const_cast<segment *>(&(*vj)));
				}
			}
		}
	}
	for(map< int, set<segment> >::iterator vi = ver.m.begin() ; vi != ver.m.end() ; ++vi) {
		set<segment>::iterator vj;
		for(vj = vi->second.begin() ; vj != vi->second.end() ; ++vj) {
			addExtremity(point(vj->o.x,vj->o.y), NULL, const_cast<segment *>(&(*vj)));
			addExtremity(point(vj->d.x,vj->d.y), NULL, const_cast<segment *>(&(*vj)));
		}
	}
	for(map< int, set<segment> >::iterator hi = hor.m.begin() ; hi != hor.m.end() ; ++hi) {
		set<segment>::iterator hj;
		for(hj = hi->second.begin() ; hj != hi->second.end() ; ++hj) {
			addExtremity(point(hj->o.x,hj->o.y), const_cast<segment *>(&(*hj)), NULL);
			addExtremity(point(hj->d.x,hj->d.y), const_cast<segment *>(&(*hj)), NULL);
		}
	}
	for(set<intersection>::iterator i = itrscts.begin() ; i != itrscts.end() ; ++i) {
		intersection * ii = const_cast<intersection *> (&(*i));
		ii->l = find_l(ii);
		ii->r = find_r(ii);
		ii->d = find_d(ii);
		ii->u = find_u(ii);
	}
	int ret = 0;
	for(set<intersection>::iterator i = itrscts.begin() ; i != itrscts.end() ; ++i) {
		if(!i->visited) {
			int eulerPaths = visit(&(*i))/2;
			if(!eulerPaths) eulerPaths = 1;
			ret += eulerPaths;
		}
	}
	return ret - 1;
}