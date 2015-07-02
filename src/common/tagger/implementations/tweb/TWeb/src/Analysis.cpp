#include "Analysis.h"
_ACT SAnalysis::
GetTopAction()
{
	double maxScore = -1.7e308;
	int maxIdx = -1;
	for (int i = 0; i < m_pSen->Length(); ++i)
	{
		if (m_pSen->Tagged(i) == false)
		{
			if (m_pActArys[i]->m_pAry[0].m_score > maxScore)
			{
				maxScore = m_pActArys[i]->m_pAry[0].m_score;
				maxIdx = i;
			}
		}
	}

	assert(maxIdx != -1);
	return *m_pActArys[maxIdx]->m_pAry;
}

