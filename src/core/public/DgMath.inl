namespace Dg
{
	namespace impl
	{
		double const C_INVERF[] =
		{
			0.88622692545275805, 0.23201366653465452, 0.12755617530559799, 0.086552129241547565,
			0.064959617745385445, 0.051731281984616399, 0.042836720651797375, 0.036465929308531653,
			0.031689005021605474, 0.027980632964995245, 0.025022275841198368, 0.022609863318897597,
			0.020606780379059025, 0.018918217250778881, 0.017476370562856568, 0.016231500987685273,
			0.015146315063247829, 0.014192316002509983, 0.013347364197421319, 0.01259400487133209,
			0.011918295936392058, 0.01130897010592256, 0.010756825303317979, 0.01025427408185349,
			0.0097950057700712002, 0.0093737298191821066, 0.0089859785028434062, 0.0086279535807094604,
			0.0082964059277392636, 0.007988540162603381, 0.0077019384322597889, 0.0074344990178315581,
			0.0071843865112683342, 0.0069499911010647356, 0.0067298950853415529, 0.0065228451614500853,
			0.0063277293643431666, 0.0061435577703841801, 0.0059694462697344802, 0.0058046028538343234,
			0.0056483159755515841, 0.0054999446262039894, 0.0053589098416864682, 0.0052246874036875109,
			0.0050968015447062252, 0.0049748194997390601, 0.0048583467749585066, 0.0047470230258849968,
			0.0046405184555590511, 0.0045385306579071071, 0.0044407818435272042, 0.0043470163950215307,
			0.0042569987071827504, 0.0041705112741261931, 0.0040873529911090445, 0.0040073376434981642,
			0.0039302925593065006, 0.0038560574050482401, 0.0037844831074738501, 0.0037154308861260723,
			0.0036487713836791174, 0.0035843838827445923, 0.0035221555992978907, 0.0034619810441376799,
			0.0034037614448720927, 0.0033474042218555873, 0.003292822512303339, 0.0032399347375042528,
			0.0031886642086556508, 0.0031389387673655968, 0.0030906904573240641, 0.0030438552240413799,
			0.0029983726398996359, 0.0029541856520668682, 0.0029112403510908463, 0.002869485758223855,
			0.0028288736297367579, 0.0027893582766628736, 0.0027508963985735458, 0.0027134469301298612,
			0.0026769708992817219, 0.0026414312960977254, 0.0026067929513093335, 0.0025730224237419642,
			0.0025400878958849404, 0.0025079590769233537, 0.0024766071126183305, 0.0024460045014791207,
			0.0024161250167213989, 0.0023869436335520787, 0.0023584364613620829, 0.0023305806804456639,
			0.0023033544828983195, 0.0022767370173755247, 0.0022507083374217769, 0.0022252493531041686,
			0.0022003417857069845, 0.0021759681252641284, 0.0021521115907245565, 0.0021287560925625647,
			0.0021058861976599873, 0.0020834870963012055, 0.0020615445711343909, 0.0020400449679639685,
			0.0020189751682497124, 0.0019983225631974915, 0.0019780750293354201, 0.0019582209054772029,
			0.0019387489709817977, 0.0019196484252252471, 0.0019009088682066932, 0.0018825202822163097,
			0.0018644730144980277, 0.0018467577608447807, 0.0018293655500683866, 0.0018122877292902292,
			0.0017955159500026982, 0.0017790421548547414, 0.001762858565118115, 0.0017469576687938568,
			0.0017313322093212079, 0.0017159751748537784, 0.0017008797880700072, 0.0016860394964872216,
			0.0016714479632505255, 0.0016570990583696158, 0.0016429868503784189, 0.0016291055983939391,
			0.0016154497445522436, 0.0016020139068009349, 0.0015887928720286467, 0.0015757815895133939,
			0.0015629751646726643, 0.0015503688530991901, 0.001537958054867315, 0.001525738309095767,
			0.0015137052887534929, 0.0015018547956959926, 0.0014901827559203322, 0.0014786852150277092,
			0.0014673583338830488, 0.0014561983844617668, 0.0014452017458743696, 0.0014343649005600465,
			0.0014236844306410165, 0.0014131570144297033, 0.0014027794230813966, 0.0013925485173853513,
			0.0013824612446877126, 0.0013725146359400136, 0.0013627058028673094, 0.0013530319352503351,
			0.0013434902983163991, 0.0013340782302339431, 0.0013247931397060433, 0.0013156325036582962,
			0.0013065938650168492, 0.0012976748305724597, 0.0012888730689267682, 0.0012801863085171127,
			0.0012716123357163986, 0.0012631489930047385, 0.0012547941772097137, 0.0012465458378122846,
			0.0012384019753155159, 0.0012303606396734165, 0.0012224199287773287, 0.0012145779869974309,
			0.0012068330037770414, 0.0011991832122774809, 0.0011916268880714288, 0.0011841623478827256,
			0.0011767879483707399, 0.001169502084957461, 0.0011623031906955791, 0.0011551897351759013,
			0.001148160223472514, 0.0011412131951241838, 0.0011343472231505616, 0.0011275609131018043,
			0.0011208529021403024, 0.0011142218581532612, 0.0011076664788949311, 0.0011011854911573368,
			0.001094777649968426, 0.0010884417378165601, 0.0010821765639003661, 0.0010759809634029848,
			0.0010698537967897912, 0.0010637939491287135, 0.0010578003294322966, 0.0010518718700207297,
			0.0010460075259050299, 0.0010402062741896763, 0.0010344671134939597, 0.0010287890633913783,
			0.0010231711638664324, 0.0010176124747881825, 0.0010121120753999768, 0.0010066690638247727,
			0.0010012825565854965, 0.00099595168813992039, 0.00099067561042953905, 0.00098545349244195338,
			0.00098028451978631305, 0.00097516789428134167, 0.00097010283355552845, 0.00096508857065906526,
			0.00096012435368712931, 0.0009552094454141248, 0.00095034312293852099, 0.00094552467733792251,
			0.00094075341333402945, 0.00093602864896717502, 0.00093134971528010656, 0.00092671595601069915,
			0.000922126727293341, 0.00091758139736867301, 0.00091307934630142776, 0.00090861996570610498,
			0.00090420265848023616, 0.00089982683854497277, 0.00089549193059279639, 0.00089119736984210403,
			0.00088694260179844836, 0.00088272708202224482, 0.00087855027590271492, 0.00087441165843790214,
			0.00087031071402053541, 0.00086624693622959189, 0.00086221982762737332, 0.00085822889956191297,
			0.00085427367197457441, 0.00085035367321266116, 0.00084646843984690105, 0.00084261751649365561,
			0.00083880045564171111, 0.00083501681748350098, 0.00083126616975066491, 0.00082754808755377022,
			0.00082386215322610104, 0.00082020795617139617, 0.00081658509271540522, 0.00081299316596116151,
			0.00080943178564786122, 0.00080590056801325071, 0.00080239913565940599, 0.00079892711742182807,
			0.00079548414824173589, 0.00079206986904149215, 0.00078868392660304406, 0.00078532597344932247,
			0.00078199566772849452, 0.00077869267310100483, 0.00077541665862931213, 0.00077216729867026488,
			0.00076894427277002936, 0.0007657472655615028, 0.00076257596666414612, 0.00075943007058616664,
			0.00075630927662899308, 0.00075321328879397243, 0.00075014181569123175, 0.00074709457045064767,
			0.00074407127063487357, 0.00074107163815434842, 0.00073809539918426325, 0.00073514228408341016,
			0.00073221202731487594, 0.00072930436736852325, 0.00072641904668522791, 0.00072355581158280603,
			0.00072071441218360508, 0.00071789460234369972, 0.00071509613958366692, 0.00071231878502088476,
			0.00070956230330333119, 0.00070682646254482809, 0.0007041110342617049, 0.00070141579331084707,
			0.00069874051782907801, 0.00069608498917386676, 0.00069344899186530565, 0.00069083231352934198,
			0.0006882347448422104, 0.00068565607947606872, 0.00068309611404578172, 0.00068055464805682846,
			0.0006780314838543129, 0.000675526426573053, 0.00067303928408870791, 0.00067056986696993246,
			0.00066811798843152096, 0.00066568346428853245, 0.0006632661129113577, 0.00066086575518171616,
			0.00065848221444955496, 0.00065611531649083167, 0.00065376488946615745, 0.00065143076388028134,
			0.00064911277254239852, 0.00064681075052725433, 0.00064452453513703857, 0.00064225396586404051,
			0.00063999888435404845, 0.00063775913437048751, 0.0006355345617592603, 0.00063332501441429019,
			0.0006311303422437412, 0.00062895039713690602, 0.00062678503293174342, 0.00062463410538304376,
			0.00062249747213122654, 0.00062037499267173315, 0.00061826652832501906, 0.00061617194220711798,
			0.00061409109920078592, 0.00061202386592718332, 0.00060997011071810877, 0.00060792970358875919,
			0.00060590251621100446, 0.0006038884218871743, 0.00060188729552433391, 0.00059989901360904996,
			0.00059792345418262704, 0.00059596049681680415, 0.00059401002258990972, 0.00059207191406346056,
			0.00059014605525918801, 0.00058823233163649566, 0.00058633063007032841, 0.00058444083882945076,
			0.00058256284755511996, 0.00058069654724015153, 0.00057884183020836253, 0.00057699859009439165,
			0.0005751667218238817, 0.00057334612159402084, 0.0005715366868544354, 0.0005697383162884198,
			0.00056795090979451046, 0.00056617436846838061, 0.00056440859458506214, 0.00056265349158147971,
			0.00056090896403929549, 0.00055917491766805467, 0.00055745125928862674, 0.00055573789681694096,
			0.00055403473924799888, 0.00055234169664017429, 0.00055065868009977704, 0.00054898560176589036,
			0.00054732237479546771, 0.00054566891334868804, 0.00054402513257455903, 0.00054239094859677005,
			0.00054076627849978707, 0.00053915104031518118, 0.00053754515300819211, 0.00053594853646451936,
			0.00053436111147733519, 0.00053278279973451438, 0.00053121352380608498, 0.00052965320713188201,
			0.00052810177400940963, 0.00052655914958191143, 0.0005250252598266277, 0.00052350003154325649,
			0.00052198339234260269, 0.00052047527063541106, 0.00051897559562138866, 0.00051748429727840081,
			0.00051600130635184893, 0.00051452655434421708, 0.00051305997350479301, 0.00051160149681955087,
			0.00051015105800120218, 0.00050870859147940503, 0.00050727403239112965, 0.00050584731657118162,
			0.00050442838054287143, 0.00050301716150884105, 0.00050161359734202863, 0.00050021762657678239,
			0.00049882918840010726, 0.00049744822264306322, 0.00049607466977228515, 0.00049470847088164233,
			0.00049334956768403273, 0.00049199790250329116, 0.00049065341826624647, 0.00048931605849487881,
			0.00048798576729861237, 0.00048666248936672429, 0.00048534616996087046, 0.00048403675490772441,
			0.0004827341905917336, 0.0004814384239479805, 0.00048014940245515918, 0.00047886707412865434,
			0.00047759138751372735, 0.00047632229167880724, 0.00047505973620887967, 0.00047380367119897652,
			0.00047255404724776373, 0.00047131081545122977, 0.00047007392739646095, 0.00046884333515551563,
			0.00046761899127938566, 0.00046640084879205135, 0.00046518886118462205, 0.00046398298240956233,
			0.00046278316687500755, 0.00046158936943916122, 0.00046040154540477141, 0.00045921965051369258,
			0.0004580436409415237, 0.00045687347329232715, 0.00045570910459342355, 0.00045455049229025995,
			0.00045339759424135526, 0.00045225036871331787, 0.00045110877437593412, 0.00044997277029732824,
			0.00044884231593918944, 0.00044771737115207244, 0.00044659789617075915, 0.00044548385160969324,
			0.00044437519845846895, 0.00044327189807739399, 0.00044217391219311398, 0.00044108120289428987,
			0.00043999373262734818, 0.00043891146419228135, 0.00043783436073851188, 0.00043676238576081445,
			0.00043569550309529117, 0.000434633676915407, 0.00043357687172807679, 0.00043252505236981092,
			0.0004314781840029097, 0.00043043623211170748, 0.00042939916249888068, 0.00042836694128179471,
			0.00042733953488890465, 0.00042631691005620524, 0.00042529903382372948, 0.00042428587353209762,
			0.00042327739681910487, 0.00042227357161636278, 0.00042127436614598698, 0.00042027974891731989,
			0.00041928968872370859, 0.00041830415463932043, 0.00041732311601599975, 0.00041634654248017347,
			0.00041537440392978782, 0.00041440667053129431, 0.0004134433127166766, 0.00041248430118050373,
			0.00041152960687704064, 0.00041057920101738243, 0.0004096330550666322, 0.0004086911407411153,
			0.00040775343000563349, 0.00040681989507074706, 0.00040589050839009771, 0.00040496524265776885,
			0.00040404407080567062, 0.00040312696600097188, 0.00040221390164355513, 0.0004013048513635071,
			0.00040039978901864443, 0.00039949868869206516, 0.00039860152468973887, 0.00039770827153812514,
			0.00039681890398181725, 0.00039593339698122302, 0.00039505172571027307, 0.00039417386555415336,
			0.00039329979210707698, 0.00039242948117006914, 0.00039156290874879619, 0.00039070005105140996,
			0.00038984088448642862, 0.00038898538566062909, 0.0003881335313769852, 0.00038728529863261925,
			0.00038644066461677887, 0.00038559960670884623, 0.00038476210247636808, 0.000383928129673103,
			0.00038309766623710893, 0.00038227069028884094, 0.00038144718012927422, 0.00038062711423805508,
			0.00037981047127167296, 0.00037899723006165034, 0.00037818736961276177, 0.00037738086910126808,
			0.00037657770787317805, 0.0003757778654425233, 0.00037498132148966457, 0.00037418805585960873,
			0.00037339804856035207, 0.00037261127976124077, 0.00037182772979135216, 0.00037104737913789453,
			0.00037027020844462838, 0.00036949619851030311, 0.00036872533028711437, 0.00036795758487917897
		};
	}
}