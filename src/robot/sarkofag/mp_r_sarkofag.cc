/*!
 * @file
 * @brief File contains mp robot class definition for Sarkofag
 * @author twiniars <twiniars@ia.pw.edu.pl>, Warsaw University of Technology
 *
 * @ingroup sarkofag
 */

#include "robot/sarkofag/mp_r_sarkofag.h"

namespace mrrocpp {
namespace mp {
namespace robot {

sarkofag::sarkofag(task::task &mp_object_l) :
	mp::robot::robot(lib::sarkofag::ROBOT_NAME, mp_object_l, lib::sarkofag::NUM_OF_SERVOS)
{
}

} // namespace robot
} // namespace mp
} // namespace mrrocpp
