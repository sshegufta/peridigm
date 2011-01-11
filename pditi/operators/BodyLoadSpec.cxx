/*
 * BodyLoadSpec.cxx
 *
 *  Created on: Jun 18, 2010
 *      Author: jamitch
 */
#include "BodyLoadSpec.h"
#include "PdImpOperatorUtilities.h"

namespace PdImp {

BodyLoadSpec::BodyLoadSpec(double unitDirection[3], const Pd_shared_ptr_Array<int>& pointIds)
:
		localIds(pointIds)
{
	u[0]=unitDirection[0];
	u[1]=unitDirection[1];
	u[2]=unitDirection[2];
}

shared_ptr<Loader> BodyLoadSpec::getStageLoader(const StageFunction& stageFunction) const {
	return shared_ptr<Loader>(new BodyLoader(*this,stageFunction));
}

BodyLoadSpec::BodyLoader::BodyLoader(const BodyLoadSpec& spec, const StageFunction& stageFunction)
:
		stageFunction(stageFunction), localIds(spec.localIds)
{
	u[0] = spec.u[0];
	u[1] = spec.u[1];
	u[2] = spec.u[2];
}

void BodyLoadSpec::BodyLoader::computeOwnedExternalForce(double lambda, Field_NS::Field<double> force) const {
	double bF[3];
	double *fHead = force.getArray().get();
	double *f;
	double magnitude = stageFunction.value(lambda);
	for(const int *p = localIds.get();p!=localIds.end();p++){

		int id = *p;
		/*
		 * bF = u
		 */
		PdImp::COPY(u,u+3,bF);

		/*
		 * bF = b * bF = b * u
		 */
		PdImp::SCALE_BY_VALUE(bF,bF+3,magnitude);

		f = fHead + 3 * id;

		/*
		 * f = f + bF
		 */
		PdImp::SUMINTO(bF,bF+3,f);

	}

}
}