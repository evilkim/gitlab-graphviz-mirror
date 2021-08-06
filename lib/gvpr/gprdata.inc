X(V_, this,      "$",            ID,       T_obj,              0,           YALL)
X(V_, thisg,     "$G",           ID,       T_graph,            0,           Y(G))
X(V_, nextg,     "$NG",          ID,       T_graph,            0,           Y(G))
X(V_, targt,     "$T",           ID,       T_graph,            0,           Y(G))
X(V_, outgraph,  "$O",           ID,       T_graph,            0,           Y(G))
X(V_, tgtname,   "$tgtname",     ID,       STRING,             0,           Y(S))
X(V_, infname,   "$F",           ID,       STRING,             0,           Y(S))
X(V_, travroot,  "$tvroot",      ID,       T_node,             0,           Y(V))
X(V_, travnext,  "$tvnext",      ID,       T_node,             0,           Y(V))
X(V_, travedge,  "$tvedge",      ID,       T_edge,             0,           Y(E))
X(V_, travtype,  "$tvtype",      ID,       T_tvtyp,            0,           Y(TV))
X(V_, ARGC,      "ARGC",         ID,       INTEGER,            0,           Y(I))
X(M_, degree,    "degree",       ID,       INTEGER,            Y(V),        Y(I))
X(M_, X,         "X",            ID,       FLOATING,           Y(V),        Y(F))
X(M_, Y,         "Y",            ID,       FLOATING,           Y(V),        Y(F))
X(M_, head,      "head",         ID,       T_node,             Y(E),        Y(V))
X(M_, tail,      "tail",         ID,       T_node,             Y(E),        Y(V))
X(M_, name,      "name",         ID,       STRING,             YALL,        Y(S))
X(M_, indegree,  "indegree",     ID,       INTEGER,            Y(V),        Y(I))
X(M_, outdegree, "outdegree",    ID,       INTEGER,            Y(V),        Y(I))
X(M_, root,      "root",         ID,       T_graph,            YALL,        Y(G))
X(M_, parent,    "parent",       ID,       T_graph,            Y(G),        Y(G))
X(M_, n_edges,   "n_edges",      ID,       INTEGER,            Y(G),        Y(I))
X(M_, n_nodes,   "n_nodes",      ID,       INTEGER,            Y(G),        Y(I))
X(M_, directed,  "directed",     ID,       INTEGER,            Y(G),        Y(I))
X(M_, strict,    "strict",       ID,       INTEGER,            Y(G),        Y(I))
X(T_, node,      "node_t",       DECLARE,  T_node)
X(T_, edge,      "edge_t",       DECLARE,  T_edge)
X(T_, graph,     "graph_t",      DECLARE,  T_graph)
X(T_, obj,       "obj_t",        DECLARE,  T_obj)
X(T_, tvtyp,     "tvtype_t",     DECLARE,  T_tvtyp)
X(A_, ARGV,      "ARGV",         ARRAY,    S|A(1,I))
X(F_, graph,     "graph",        FUNCTION, G|A(1,S)|A(2,S))
X(F_, subg,      "subg",         FUNCTION, G|A(1,G)|A(2,S))
X(F_, issubg,    "isSubg",       FUNCTION, G|A(1,G)|A(2,S))
X(F_, fstsubg,   "fstsubg",      FUNCTION, G|A(1,G))
X(F_, nxtsubg,   "nxtsubg",      FUNCTION, G|A(1,G))
X(F_, node,      "node",         FUNCTION, V|A(1,G)|A(2,S))
X(F_, addnode,   "subnode",      FUNCTION, V|A(1,G)|A(2,V))
X(F_, fstnode,   "fstnode",      FUNCTION, V|A(1,G))
X(F_, nxtnode,   "nxtnode",      FUNCTION, V|A(1,V))
X(F_, nxtnodesg, "nxtnode_sg",   FUNCTION, V|A(1,G)|A(2,V))
X(F_, isnode,    "isNode",       FUNCTION, V|A(1,G)|A(2,S))
X(F_, issubnode, "isSubnode",    FUNCTION, I|A(1,G)|A(2,V))
X(F_, indegree,  "indegreeOf",   FUNCTION, I|A(1,G)|A(2,V))
X(F_, outdegree, "outdegreeOf",  FUNCTION, I|A(1,G)|A(2,V))
X(F_, degree,    "degreeOf",     FUNCTION, I|A(1,G)|A(2,V))
X(F_, isin,      "isIn",         FUNCTION, I|A(1,G)|A(2,O))
X(F_, edge,      "edge",         FUNCTION, E|A(1,V)|A(2,V)|A(3,S))
X(F_, edgesg,    "edge_sg",      FUNCTION, E|A(1,G)|A(2,V)|A(3,V)|A(4,S))
X(F_, addedge,   "subedge",      FUNCTION, E|A(1,G)|A(2,E))
X(F_, opp,       "opp",          FUNCTION, V|A(1,E)|A(2,V))
X(F_, fstout,    "fstout",       FUNCTION, E|A(1,V))
X(F_, nxtout,    "nxtout",       FUNCTION, E|A(1,E))
X(F_, fstin,     "fstin",        FUNCTION, E|A(1,V))
X(F_, nxtin,     "nxtin",        FUNCTION, E|A(1,E))
X(F_, fstedge,   "fstedge",      FUNCTION, E|A(1,V))
X(F_, nxtedge,   "nxtedge",      FUNCTION, E|A(1,E)|A(2,V))
X(F_, fstoutsg,  "fstout_sg",    FUNCTION, E|A(1,G)|A(2,V))
X(F_, nxtoutsg,  "nxtout_sg",    FUNCTION, E|A(1,G)|A(2,E))
X(F_, fstinsg,   "fstin_sg",     FUNCTION, E|A(1,G)|A(2,V))
X(F_, nxtinsg,   "nxtin_sg",     FUNCTION, E|A(1,G)|A(2,E))
X(F_, fstedgesg, "fstedge_sg",   FUNCTION, E|A(1,G)|A(2,V))
X(F_, nxtedgesg, "nxtedge_sg",   FUNCTION, E|A(1,G)|A(2,E)|A(3,V))
X(F_, compof,    "compOf",       FUNCTION, G|A(1,G)|A(2,V))
X(F_, kindof,    "kindOf",       FUNCTION, S|A(1,O))
X(F_, index,     "index",        FUNCTION, I|A(1,S)|A(2,S))
X(F_, rindex,    "rindex",       FUNCTION, I|A(1,S)|A(2,S))
X(F_, isedge,    "isEdge",       FUNCTION, E|A(1,V)|A(2,V)|A(3,S))
X(F_, isedgesg,  "isEdge_sg",    FUNCTION, E|A(1,G)|A(2,V)|A(3,V)|A(4,S))
X(F_, issubedge, "isSubedge",    FUNCTION, I|A(1,G)|A(2,E))
X(F_, length,    "length",       FUNCTION, I|A(1,S))
X(F_, match,     "match",        FUNCTION, I|A(1,S)|A(2,S))
X(F_, write,     "write",        FUNCTION, I|A(1,G))
X(F_, writeg,    "writeG",       FUNCTION, I|A(1,G)|A(2,S))
X(F_, readg,     "readG",        FUNCTION, G|A(1,S))
X(F_, fwriteg,   "fwriteG",      FUNCTION, I|A(1,G)|A(2,I))
X(F_, freadg,    "freadG",       FUNCTION, G|A(1,I))
X(F_, openf,     "openF",        FUNCTION, I|A(1,S)|A(2,S))
X(F_, closef,    "closeF",       FUNCTION, I|A(1,I))
X(F_, readl,     "readL",        FUNCTION, S|A(1,I))
X(F_, induce,    "induce",       FUNCTION, I|A(1,G))
X(F_, isdirect,  "isDirect",     FUNCTION, I|A(1,G))
X(F_, isstrict,  "isStrict",     FUNCTION, I|A(1,G))
X(F_, delete,    "delete",       FUNCTION, I|A(1,G)|A(2,O))
X(F_, clone,     "clone",        FUNCTION, O|A(1,G)|A(2,O))
X(F_, cloneG,    "cloneG",       FUNCTION, G|A(1,G)|A(2,S))
X(F_, copy,      "copy",         FUNCTION, O|A(1,G)|A(2,O))
X(F_, copya,     "copyA",        FUNCTION, I|A(1,O)|A(2,O))
X(F_, lock,      "lock",         FUNCTION, I|A(1,G)|A(2,I))
X(F_, nnodes,    "nNodes",       FUNCTION, I|A(1,G))
X(F_, nedges,    "nEdges",       FUNCTION, I|A(1,G))
X(F_, sqrt,      "sqrt",         FUNCTION, F|A(1,F))
X(F_, cos,       "cos",          FUNCTION, F|A(1,F))
X(F_, sin,       "sin",          FUNCTION, F|A(1,F))
X(F_, atan2,     "atan2",        FUNCTION, F|A(1,F)|A(2,F))
X(F_, exp,       "exp",          FUNCTION, F|A(1,F))
X(F_, pow,       "pow",          FUNCTION, F|A(1,F)|A(2,F))
X(F_, log,       "log",          FUNCTION, F|A(1,F))
X(F_, min,       "MIN",          FUNCTION, F|A(1,F)|A(2,F))
X(F_, max,       "MAX",          FUNCTION, F|A(1,F)|A(2,F))
X(F_, sys,       "system",       FUNCTION, I|A(1,S))
X(F_, xof,       "xOf",          FUNCTION, S|A(1,S))
X(F_, yof,       "yOf",          FUNCTION, S|A(1,S))
X(F_, llof,      "llOf",         FUNCTION, S|A(1,S))
X(F_, urof,      "urOf",         FUNCTION, S|A(1,S))
X(F_, html,      "html",         FUNCTION, S|A(1,G)|A(2,S))
X(F_, ishtml,    "ishtml",       FUNCTION, I|A(1,S))
X(F_, canon,     "canon",        FUNCTION, S|A(1,S))
X(F_, get,       "aget",         FUNCTION, S|A(1,O)|A(2,S))
X(F_, set,       "aset",         FUNCTION, I|A(1,O)|A(2,S)|A(3,S))
X(F_, dget,      "getDflt",      FUNCTION, S|A(1,G)|A(2,S)|A(3,S))
X(F_, dset,      "setDflt",      FUNCTION, I|A(1,G)|A(2,S)|A(3,S)|A(4,S))
X(F_, hasattr,   "hasAttr",      FUNCTION, I|A(1,O)|A(2,S))
X(F_, isattr,    "isAttr",       FUNCTION, I|A(1,G)|A(2,S)|A(3,S))
X(F_, fstattr,   "fstAttr",      FUNCTION, S|A(1,G)|A(2,S))
X(F_, nxtattr,   "nxtAttr",      FUNCTION, S|A(1,G)|A(2,S)|A(3,S))
X(F_, tolower,   "tolower",      FUNCTION, S|A(1,S))
X(F_, toupper,   "toupper",      FUNCTION, S|A(1,S))
X(F_, strcmp,    "strcmp",       FUNCTION, I|A(1,S)|A(2,S))
X(F_, atoi,      "atoi",         FUNCTION, I|A(1,S))
X(F_, atof,      "atof",         FUNCTION, F|A(1,S))
X(F_, colorx,    "colorx",       FUNCTION, S|A(1,S)|A(2,S))
X(F_, call,      "call",         FUNCTION, I|A(1,S)|A(2,S))
X(C_, flat,      "TV_flat",      CONSTANT, T_tvtyp)
X(C_, ne,        "TV_ne",        CONSTANT, T_tvtyp)
X(C_, en,        "TV_en",        CONSTANT, T_tvtyp)
X(C_, bfs,       "TV_bfs",       CONSTANT, T_tvtyp)
X(C_, dfs,       "TV_dfs",       CONSTANT, T_tvtyp)
X(C_, fwd,       "TV_fwd",       CONSTANT, T_tvtyp)
X(C_, rev,       "TV_rev",       CONSTANT, T_tvtyp)
X(C_, postdfs,   "TV_postdfs",   CONSTANT, T_tvtyp)
X(C_, postfwd,   "TV_postfwd",   CONSTANT, T_tvtyp)
X(C_, postrev,   "TV_postrev",   CONSTANT, T_tvtyp)
X(C_, prepostdfs,   "TV_prepostdfs",   CONSTANT, T_tvtyp)
X(C_, prepostfwd,   "TV_prepostfwd",   CONSTANT, T_tvtyp)
X(C_, prepostrev,   "TV_prepostrev",   CONSTANT, T_tvtyp)
X(C_, null,      "NULL",         CONSTANT, T_obj)