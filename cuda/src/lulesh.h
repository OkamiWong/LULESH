#include "vector.h"

#define LULESH_SHOW_PROGRESS 0
#define DOUBLE_PRECISION

enum {
  VolumeError = -1,
  QStopError = -2,
  LFileError = -3
};

/* Could also support fixed point and interval arithmetic types */
typedef float real4;
typedef double real8;

typedef int Index_t; /* array subscript and loop index */
typedef int Int_t;   /* integer representation */
#ifdef DOUBLE_PRECISION
typedef real8 Real_t; /* floating point representation */
#else
typedef real4 Real_t; /* floating point representation */
#endif

class Domain {
 public:
  void sortRegions(Vector_h<Int_t>& regReps_h, Vector_h<Index_t>& regSorted_h);
  void CreateRegionIndexSets(Int_t nr, Int_t balance);

  Index_t max_streams;
  std::vector<cudaStream_t> streams;

  /* Elem-centered */

  Index_t* matElemlist; /* material indexset */
  Index_t* nodelist;    /* elemToNode connectivity */

  Index_t* lxim; /* element connectivity through face */
  Index_t* lxip;
  Index_t* letam;
  Index_t* letap;
  Index_t* lzetam;
  Index_t* lzetap;

  Int_t* elemBC; /* elem face symm/free-surf flag */

  Real_t* e; /* energy */

  Real_t* p; /* pressure */

  Real_t* q;  /* q */
  Real_t* ql; /* linear term for q */
  Real_t* qq; /* quadratic term for q */

  Real_t* v; /* relative volume */

  Real_t* volo; /* reference volume */
  Real_t* delv; /* m_vnew - m_v */
  Real_t* vdov; /* volume derivative over volume */

  Real_t* arealg; /* char length of an element */

  Real_t* ss; /* "sound speed" */

  Real_t* elemMass; /* mass */

  Vector_d<Real_t>* vnew; /* new relative volume -- temporary */

  Vector_d<Real_t>* delv_xi; /* velocity gradient -- temporary */
  Vector_d<Real_t>* delv_eta;
  Vector_d<Real_t>* delv_zeta;

  Vector_d<Real_t>* delx_xi; /* coordinate gradient -- temporary */
  Vector_d<Real_t>* delx_eta;
  Vector_d<Real_t>* delx_zeta;

  Vector_d<Real_t>* dxx; /* principal strains -- temporary */
  Vector_d<Real_t>* dyy;
  Vector_d<Real_t>* dzz;

  /* Node-centered */

  Real_t* x; /* coordinates */
  Real_t* y;
  Real_t* z;

  Real_t* xd; /* velocities */
  Real_t* yd;
  Real_t* zd;

  Real_t* xdd; /* accelerations */
  Real_t* ydd;
  Real_t* zdd;

  Real_t* fx; /* forces */
  Real_t* fy;
  Real_t* fz;

  Real_t* nodalMass; /* mass */

  /* Boundary nodesets */

  Index_t* symmX; /* symmetry plane nodesets */
  Index_t* symmY;
  Index_t* symmZ;

  Int_t* nodeElemCount;
  Int_t* nodeElemStart;
  Index_t* nodeElemCornerList;

  /* Parameters */

  Real_t dtfixed; /* fixed time increment */
  Real_t deltatimemultlb;
  Real_t deltatimemultub;
  Real_t stoptime; /* end time for simulation */
  Real_t dtmax;    /* maximum allowable time increment */
  Int_t cycle;     /* iteration count for simulation */

  Real_t* dthydro_h;   /* hydro time constraint */
  Real_t* dtcourant_h; /* courant time constraint */
  Index_t* bad_q_h;    /* flag to indicate Q error */
  Index_t* bad_vol_h;  /* flag to indicate volume error */

  /* cuda Events to indicate completion of certain kernels */
  cudaEvent_t time_constraint_computed;

  Real_t time_h;      /* current time */
  Real_t deltatime_h; /* variable time increment */

  Real_t u_cut;  /* velocity tolerance */
  Real_t hgcoef; /* hourglass control */
  Real_t qstop;  /* excessive q indicator */
  Real_t monoq_max_slope;
  Real_t monoq_limiter_mult;
  Real_t e_cut; /* energy tolerance */
  Real_t p_cut; /* pressure tolerance */
  Real_t ss4o3;
  Real_t q_cut;     /* q tolerance */
  Real_t v_cut;     /* relative volume tolerance */
  Real_t qlc_monoq; /* linear term coef for q */
  Real_t qqc_monoq; /* quadratic term coef for q */
  Real_t qqc;
  Real_t eosvmax;
  Real_t eosvmin;
  Real_t pmin;    /* pressure floor */
  Real_t emin;    /* energy floor */
  Real_t dvovmax; /* maximum allowable volume change */
  Real_t refdens; /* reference density */

  Index_t m_colLoc;
  Index_t m_rowLoc;
  Index_t m_planeLoc;
  Index_t m_tp;

  Index_t& colLoc() { return m_colLoc; }
  Index_t& rowLoc() { return m_rowLoc; }
  Index_t& planeLoc() { return m_planeLoc; }
  Index_t& tp() { return m_tp; }

  Index_t sizeX;
  Index_t sizeY;
  Index_t sizeZ;
  Index_t maxPlaneSize;
  Index_t maxEdgeSize;

  Index_t numElem;
  Index_t padded_numElem;

  Index_t numNode;
  Index_t padded_numNode;

  Index_t numSymmX;
  Index_t numSymmY;
  Index_t numSymmZ;

  Index_t octantCorner;

  // Region information
  Int_t numReg;          // number of regions (def:11)
  Int_t balance;         // Load balance between regions of a domain (def: 1)
  Int_t cost;            // imbalance cost (def: 1)
  Int_t* regElemSize;    // Size of region sets
  Int_t* regCSR;         // records the begining and end of each region
  Int_t* regReps;        // records the rep number per region
  Index_t* regNumList;   // Region number per domain element
  Index_t* regElemlist;  // region indexset
  Index_t* regSorted;    // keeps index of sorted regions

  //
  // MPI-Related additional data
  //

  Index_t m_numRanks;
  Index_t& numRanks() { return m_numRanks; }

  void SetupCommBuffers(Int_t edgeNodes);
  void BuildMesh(Int_t nx, Int_t edgeNodes, Int_t edgeElems, Int_t domNodes, Int_t padded_domElems, Vector_h<Real_t>& x_h, Vector_h<Real_t>& y_h, Vector_h<Real_t>& z_h, Vector_h<Int_t>& nodelist_h);

  // Used in setup
  Index_t m_rowMin, m_rowMax;
  Index_t m_colMin, m_colMax;
  Index_t m_planeMin, m_planeMax;
};

typedef Real_t& (Domain::*Domain_member)(Index_t);

// Assume 128 byte coherence
// Assume Real_t is an "integral power of 2" bytes wide
#define CACHE_COHERENCE_PAD_REAL (128 / sizeof(Real_t))

#define CACHE_ALIGN_REAL(n) \
  (((n) + (CACHE_COHERENCE_PAD_REAL - 1)) & ~(CACHE_COHERENCE_PAD_REAL - 1))

// MPI Message Tags
#define MSG_COMM_SBN 1024
#define MSG_SYNC_POS_VEL 2048
#define MSG_MONOQ 3072

#define MAX_FIELDS_PER_MPI_COMM 6
