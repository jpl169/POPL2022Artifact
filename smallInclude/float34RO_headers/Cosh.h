//unsigned bitsSame = 6;
//unsigned N = 5
double __coshCoeffs[32][3] = {
	{
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		1.0000000336688383839600646751932799816131591796875000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		1.0000000013472192250674197566695511341094970703125000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		1.0000000001956956818816024679108522832393646240234375000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		1.0000000001833801999140405314392410218715667724609375000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		1.0000000003073830079358685907209292054176330566406250000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		1.0000000000308382208658031231607310473918914794921875000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		1.0000000000470110617101227035163901746273040771484375000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		1.0000000000057513993567681609420105814933776855468750000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		1.0000000000008464340339742193464189767837524414062500000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		9.9999999999911204362490479979896917939186096191406250000000000000000000000000000000000000000000000000e-01,
		5.2208564553280756292963360465364530682563781738281250000000000000000000000000000000000000000000000000e-01,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		9.9999999999969158004375913151307031512260437011718750000000000000000000000000000000000000000000000000e-01,
		5.0129560258479433088041332666762173175811767578125000000000000000000000000000000000000000000000000000e-01,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		9.9999999999979904963254284666618332266807556152343750000000000000000000000000000000000000000000000000e-01,
		5.0002360821903224508133689596434123814105987548828125000000000000000000000000000000000000000000000000e-01,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		9.9999999999997712940569272177526727318763732910156250000000000000000000000000000000000000000000000000e-01,
		5.0000013580094548881760374570148997008800506591796875000000000000000000000000000000000000000000000000e-01,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		1.0000000000000068833827526759705506265163421630859375000000000000000000000000000000000000000000000000e+00,
		4.9999999183061855090315361849206965416669845581054687500000000000000000000000000000000000000000000000e-01,
		4.3525833226465880654298956642378470860421657562255859375000000000000000000000000000000000000000000000e-02
	},
	{
		1.0000000000000028865798640254070051014423370361328125000000000000000000000000000000000000000000000000e+00,
		4.9999999971411451804570447166042868047952651977539062500000000000000000000000000000000000000000000000e-01,
		4.1671532258076703081339786649550660513341426849365234375000000000000000000000000000000000000000000000e-02
	},
	{
		1.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		4.9999999982026216427399845088075380772352218627929687500000000000000000000000000000000000000000000000e-01,
		4.1668965692338341688660818817879771813750267028808593750000000000000000000000000000000000000000000000e-02
	},
	{
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	},
	{
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00,
		0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e+00
	}
};


