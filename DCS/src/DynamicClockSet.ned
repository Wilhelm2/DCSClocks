package dcs;

network DynamicClockSet
{
    @display("bgb=935,338");
    types:
        channel Channel extends ned.DelayChannel
        {
            delay = 0ms;
        }

    submodules:
		ut:Utilitaries;
		Nodes [500]: Node;
		control : DeliveryController;
		stat	: Stats;
		centralNode : CentralCommunicationNode;
		connections:
//    for i=0..5{
	//for j=i..5{
//             	Nodes[i].neighbour++ <--> Channel <--> Nodes[j].neighbour++;
            //}
  //      }
		Nodes[0].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[1].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[2].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[3].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[4].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[5].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[6].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[7].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[8].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[9].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[10].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[11].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[12].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[13].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[14].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[15].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[16].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[17].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[18].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[19].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[20].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[21].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[22].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[23].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[24].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[25].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[26].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[27].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[28].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[29].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[30].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[31].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[32].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[33].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[34].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[35].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[36].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[37].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[38].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[39].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[40].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[41].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[42].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[43].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[44].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[45].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[46].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[47].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[48].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[49].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[50].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[51].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[52].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[53].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[54].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[55].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[56].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[57].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[58].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[59].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[60].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[61].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[62].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[63].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[64].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[65].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[66].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[67].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[68].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[69].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[70].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[71].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[72].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[73].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[74].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[75].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[76].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[77].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[78].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[79].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[80].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[81].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[82].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[83].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[84].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[85].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[86].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[87].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[88].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[89].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[90].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[91].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[92].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[93].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[94].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[95].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[96].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[97].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[98].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[99].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[100].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[101].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[102].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[103].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[104].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[105].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[106].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[107].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[108].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[109].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[110].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[111].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[112].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[113].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[114].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[115].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[116].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[117].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[118].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[119].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[120].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[121].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[122].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[123].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[124].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[125].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[126].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[127].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[128].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[129].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[130].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[131].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[132].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[133].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[134].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[135].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[136].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[137].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[138].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[139].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[140].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[141].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[142].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[143].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[144].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[145].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[146].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[147].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[148].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[149].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[150].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[151].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[152].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[153].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[154].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[155].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[156].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[157].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[158].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[159].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[160].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[161].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[162].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[163].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[164].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[165].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[166].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[167].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[168].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[169].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[170].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[171].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[172].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[173].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[174].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[175].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[176].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[177].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[178].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[179].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[180].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[181].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[182].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[183].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[184].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[185].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[186].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[187].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[188].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[189].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[190].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[191].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[192].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[193].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[194].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[195].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[196].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[197].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[198].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[199].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[200].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[201].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[202].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[203].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[204].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[205].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[206].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[207].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[208].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[209].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[210].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[211].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[212].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[213].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[214].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[215].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[216].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[217].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[218].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[219].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[220].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[221].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[222].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[223].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[224].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[225].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[226].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[227].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[228].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[229].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[230].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[231].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[232].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[233].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[234].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[235].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[236].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[237].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[238].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[239].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[240].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[241].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[242].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[243].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[244].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[245].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[246].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[247].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[248].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[249].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[250].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[251].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[252].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[253].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[254].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[255].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[256].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[257].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[258].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[259].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[260].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[261].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[262].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[263].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[264].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[265].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[266].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[267].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[268].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[269].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[270].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[271].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[272].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[273].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[274].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[275].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[276].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[277].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[278].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[279].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[280].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[281].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[282].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[283].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[284].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[285].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[286].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[287].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[288].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[289].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[290].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[291].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[292].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[293].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[294].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[295].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[296].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[297].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[298].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[299].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[300].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[301].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[302].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[303].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[304].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[305].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[306].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[307].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[308].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[309].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[310].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[311].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[312].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[313].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[314].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[315].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[316].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[317].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[318].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[319].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[320].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[321].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[322].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[323].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[324].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[325].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[326].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[327].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[328].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[329].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[330].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[331].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[332].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[333].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[334].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[335].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[336].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[337].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[338].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[339].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[340].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[341].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[342].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[343].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[344].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[345].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[346].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[347].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[348].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[349].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[350].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[351].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[352].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[353].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[354].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[355].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[356].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[357].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[358].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[359].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[360].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[361].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[362].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[363].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[364].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[365].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[366].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[367].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[368].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[369].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[370].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[371].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[372].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[373].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[374].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[375].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[376].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[377].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[378].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[379].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[380].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[381].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[382].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[383].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[384].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[385].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[386].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[387].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[388].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[389].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[390].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[391].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[392].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[393].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[394].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[395].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[396].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[397].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[398].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[399].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[400].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[401].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[402].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[403].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[404].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[405].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[406].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[407].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[408].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[409].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[410].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[411].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[412].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[413].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[414].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[415].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[416].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[417].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[418].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[419].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[420].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[421].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[422].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[423].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[424].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[425].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[426].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[427].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[428].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[429].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[430].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[431].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[432].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[433].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[434].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[435].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[436].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[437].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[438].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[439].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[440].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[441].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[442].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[443].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[444].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[445].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[446].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[447].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[448].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[449].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[450].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[451].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[452].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[453].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[454].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[455].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[456].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[457].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[458].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[459].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[460].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[461].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[462].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[463].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[464].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[465].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[466].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[467].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[468].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[469].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[470].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[471].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[472].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[473].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[474].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[475].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[476].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[477].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[478].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[479].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[480].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[481].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[482].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[483].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[484].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[485].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[486].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[487].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[488].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[489].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[490].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[491].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[492].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[493].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[494].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[495].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[496].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[497].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[498].neighbour++ <--> Channel <--> centralNode.neighbour++;
		Nodes[499].neighbour++ <--> Channel <--> centralNode.neighbour++;
}