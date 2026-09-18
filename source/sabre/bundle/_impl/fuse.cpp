/// Sabre Includes
#include "sabre/bundle/fuse.hpp"

//  PROPERTIES  //

#define XX_FUSE(N, F, ...) "SABRE_FUSE_" #N "/" F ":0"
#define XX_VIEW(N, F, ...) $_FWD(Sabre::Fuse, XJCT::Sentinel::View N = XJCT::Sentinel::Fuse<XX_FUSE(N, F)>().view())
SABRE_XX_FUSES_LIST(XX_VIEW)
#undef XX_VIEW
#undef XX_FUSE
