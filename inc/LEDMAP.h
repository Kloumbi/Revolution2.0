/*
 * LEDMAP.h
 *
 *  Created on: Apr 15, 2019
 *      Author: rev
 */

#ifndef LEDMAP_H_
#define LEDMAP_H_

/*SPI4 U3 */
#define D83	2
#define D87	6
#define D84	3
#define D88	7
#define D92	11
#define D96	15
#define D91	10
#define D95	14
#define D94	13
#define D90	9
#define D93	12
#define D89	8
#define D85	4
#define D81	0
#define D86	5
#define D82	1

/*SPI4 U2 */
#define D67	18
#define D71	22
#define D68	19
#define D72	23
#define D76	27
#define D80	31
#define D75	26
#define D79	30
#define D78	29
#define D74	25
#define D77	28
#define D73	24
#define D69	20
#define D65	16
#define D70	21
#define D66	17

/*SPI4 U1*/
#define D51	34
#define D55	38
#define D52	35
#define D56	39
#define D60	43
#define D64	47
#define D59	42
#define D63	46
#define D62	45
#define D58	41
#define D61	44
#define D57	40
#define D53	36
#define D49	32
#define D54	37
#define D50	33

/*SPI3 U3*/
#define D34	1
#define D38	5
#define D33	0
#define D37	4
#define D41	8
#define D45	12
#define D42	9
#define D46	13
#define D47	14
#define D43	10
#define D48	15
#define D44	11
#define D40	7
#define D36	3
#define D39	6
#define D35	2

/*SPI3 U2 */
#define D18	17
#define D22	21
#define D17	16
#define D21	20
#define D25	24
#define D29	28
#define D26	25
#define D30	29
#define D31	30
#define D27	26
#define D32	31
#define D28	27
#define D24	23
#define D20	19
#define D23	22
#define D19	18

/*SPI3 U1 */
#define D2	33
#define D6	37
#define D1	32
#define D5	36
#define D9	40
#define D13	44
#define D10	41
#define D14	45
#define D15	46
#define D11	42
#define D16	47
#define D12	43
#define D8	39
#define D4	35
#define D7	38
#define D3	34

/*SPI2 U3 */
#define D82	1
#define D86	5
#define D81	0
#define D85	4
#define D89	8
#define D93	12
#define D90	9
#define D94	13
#define D95	14
#define D91	10
#define D96	15
#define D92	11
#define D88	7
#define D84	3
#define D87	6
#define D83	2

/*SPI2 U2 */
#define D66	17
#define D70	21
#define D65	16
#define D69	20
#define D73	24
#define D77	28
#define D74	25
#define D78	29
#define D79	30
#define D75	26
#define D80	31
#define D76	27
#define D72	23
#define D68	19
#define D71	22
#define D67	18

/*SPI2 U1 */
#define D50	33
#define D54	37
#define D49	32
#define D53	36
#define D57	40
#define D61	44
#define D58	41
#define D62	45
#define D63	46
#define D59	42
#define D64	47
#define D60	43
#define D56	39
#define D52	35
#define D55	38
#define D51	34

/*SPI1 U3 */
#define D34	1
#define D38	5
#define D33	0
#define D37	4
#define D41	8
#define D45	12
#define D42	9
#define D46	13
#define D47	14
#define D43	10
#define D48	15
#define D44	11
#define D40	7
#define D36	3
#define D39	6
#define D35	2

/*SPI1 U2 */
#define D18	17
#define D22	21
#define D17	16
#define D21	20
#define D25	24
#define D29	28
#define D26	25
#define D30	29
#define D31	30
#define D27	26
#define D32	31
#define D28	27
#define D24	23
#define D20	19
#define D23	22
#define D19	18

/*SPI1 U1 */
#define D2	33
#define D6	37
#define D1	32
#define D5	36
#define D9	40
#define D13	44
#define D10	41
#define D14	45
#define D15	46
#define D11	42
#define D16	47
#define D12	43
#define D8	39
#define D4	35
#define D7	38
#define D3	34

#define PIXEL_ARRAY	{D51,\
D55	, \
D52	, \
D56	, \
D60	, \
D64	, \
D59	, \
D63	, \
D62	, \
D58	, \
D61	, \
D57	, \
D53	, \
D49	, \
D54	, \
D50	, \
D67	, \
D71	, \
D68	, \
D72	, \
D76	, \
D80	, \
D75	, \
D79	, \
D78	, \
D74	, \
D77	, \
D73	, \
D69	, \
D65	, \
D70	, \
D66	, \
D83	, \
D87	, \
D84	, \
D88	, \
D92	, \
D96	, \
D91	, \
D95	, \
D94	, \
D90	, \
D93	, \
D89	, \
D85	, \
D81	, \
D86	, \
D82	, \
D3	, \
D7	, \
D4	, \
D8	, \
D12	, \
D16	, \
D11	, \
D15	, \
D14	, \
D10	, \
D13	, \
D9	, \
D5	, \
D1	, \
D6	, \
D2	, \
D19	, \
D23	, \
D20	, \
D24	, \
D28	, \
D32	, \
D27	, \
D31	, \
D30	, \
D26	, \
D29	, \
D25	, \
D21	, \
D17	, \
D22	, \
D18	, \
D35	, \
D39	, \
D36	, \
D40	, \
D44	, \
D48	, \
D43	, \
D47	, \
D46	, \
D42	, \
D45	, \
D41	, \
D37	, \
D33	, \
D38	, \
D34	, \
D51	, \
D55	, \
D52	, \
D56	, \
D60	, \
D64	, \
D59	, \
D63	, \
D62	, \
D58	, \
D61	, \
D57	, \
D53	, \
D49	, \
D54	, \
D50	, \
D67	, \
D71	, \
D68	, \
D72	, \
D76	, \
D80	, \
D75	, \
D79	, \
D78	, \
D74	, \
D77	, \
D73	, \
D69	, \
D65	, \
D70	, \
D66	, \
D83	, \
D87	, \
D84	, \
D88	, \
D92	, \
D96	, \
D91	, \
D95	, \
D94	, \
D90	, \
D93	, \
D89	, \
D85	, \
D81	, \
D86	, \
D82	, \
D3	, \
D7	, \
D4	, \
D8	, \
D12	, \
D16	, \
D11	, \
D15	, \
D14	, \
D10	, \
D13	, \
D9	, \
D5	, \
D1	, \
D6	, \
D2	, \
D19	, \
D23	, \
D20	, \
D24	, \
D28	, \
D32	, \
D27	, \
D31	, \
D30	, \
D26	, \
D29	, \
D25	, \
D21	, \
D17	, \
D22	, \
D18	, \
D35	, \
D39	, \
D36	, \
D40	, \
D44	, \
D48	, \
D43	, \
D47	, \
D46	, \
D42	, \
D45	, \
D41	, \
D37	, \
D33	, \
D38	, \
D34	}


typedef enum {
	PIXEL191	=	D2	,
	PIXEL190	=	D6	,
	PIXEL189	=	D1	,
	PIXEL188	=	D5	,
	PIXEL187	=	D9	,
	PIXEL186	=	D13	,
	PIXEL185	=	D10	,
	PIXEL184	=	D14	,
	PIXEL183	=	D15	,
	PIXEL182	=	D11	,
	PIXEL181	=	D16	,
	PIXEL180	=	D12	,
	PIXEL179	=	D8	,
	PIXEL178	=	D4	,
	PIXEL177	=	D7	,
	PIXEL176	=	D3	,
	PIXEL175	=	D18	,
	PIXEL174	=	D22	,
	PIXEL173	=	D17	,
	PIXEL172	=	D21	,
	PIXEL171	=	D25	,
	PIXEL170	=	D29	,
	PIXEL169	=	D26	,
	PIXEL168	=	D30	,
	PIXEL167	=	D31	,
	PIXEL166	=	D27	,
	PIXEL165	=	D32	,
	PIXEL164	=	D28	,
	PIXEL163	=	D24	,
	PIXEL162	=	D20	,
	PIXEL161	=	D23	,
	PIXEL160	=	D19	,
	PIXEL159	=	D34	,
	PIXEL158	=	D38	,
	PIXEL157	=	D33	,
	PIXEL156	=	D37	,
	PIXEL155	=	D41	,
	PIXEL154	=	D45	,
	PIXEL153	=	D42	,
	PIXEL152	=	D46	,
	PIXEL151	=	D47	,
	PIXEL150	=	D43	,
	PIXEL149	=	D48	,
	PIXEL148	=	D44	,
	PIXEL147	=	D40	,
	PIXEL146	=	D36	,
	PIXEL145	=	D39	,
	PIXEL144	=	D35	,
	PIXEL143	=	D50	,
	PIXEL142	=	D54	,
	PIXEL141	=	D49	,
	PIXEL140	=	D53	,
	PIXEL139	=	D57	,
	PIXEL138	=	D61	,
	PIXEL137	=	D58	,
	PIXEL136	=	D62	,
	PIXEL135	=	D63	,
	PIXEL134	=	D59	,
	PIXEL133	=	D64	,
	PIXEL132	=	D60	,
	PIXEL131	=	D56	,
	PIXEL130	=	D52	,
	PIXEL129	=	D55	,
	PIXEL128	=	D51	,
	PIXEL127	=	D66	,
	PIXEL126	=	D70	,
	PIXEL125	=	D65	,
	PIXEL124	=	D69	,
	PIXEL123	=	D73	,
	PIXEL122	=	D77	,
	PIXEL121	=	D74	,
	PIXEL120	=	D78	,
	PIXEL119	=	D79	,
	PIXEL118	=	D75	,
	PIXEL117	=	D80	,
	PIXEL116	=	D76	,
	PIXEL115	=	D72	,
	PIXEL114	=	D68	,
	PIXEL113	=	D71	,
	PIXEL112	=	D67	,
	PIXEL111	=	D82	,
	PIXEL110	=	D86	,
	PIXEL109	=	D81	,
	PIXEL108	=	D85	,
	PIXEL107	=	D89	,
	PIXEL106	=	D93	,
	PIXEL105	=	D90	,
	PIXEL104	=	D94	,
	PIXEL103	=	D95	,
	PIXEL102	=	D91	,
	PIXEL101	=	D96	,
	PIXEL100	=	D92	,
	PIXEL99	=	D88	,
	PIXEL98	=	D84	,
	PIXEL97	=	D87	,
	PIXEL96	=	D83	,
	PIXEL95	=	D2	,
	PIXEL94	=	D6	,
	PIXEL93	=	D1	,
	PIXEL92	=	D5	,
	PIXEL91	=	D9	,
	PIXEL90	=	D13	,
	PIXEL89	=	D10	,
	PIXEL88	=	D14	,
	PIXEL87	=	D15	,
	PIXEL86	=	D11	,
	PIXEL85	=	D16	,
	PIXEL84	=	D12	,
	PIXEL83	=	D8	,
	PIXEL82	=	D4	,
	PIXEL81	=	D7	,
	PIXEL80	=	D3	,
	PIXEL79	=	D18	,
	PIXEL78	=	D22	,
	PIXEL77	=	D17	,
	PIXEL76	=	D21	,
	PIXEL75	=	D25	,
	PIXEL74	=	D29	,
	PIXEL73	=	D26	,
	PIXEL72	=	D30	,
	PIXEL71	=	D31	,
	PIXEL70	=	D27	,
	PIXEL69	=	D32	,
	PIXEL68	=	D28	,
	PIXEL67	=	D24	,
	PIXEL66	=	D20	,
	PIXEL65	=	D23	,
	PIXEL64	=	D19	,
	PIXEL63	=	D34	,
	PIXEL62	=	D38	,
	PIXEL61	=	D33	,
	PIXEL60	=	D37	,
	PIXEL59	=	D41	,
	PIXEL58	=	D45	,
	PIXEL57	=	D42	,
	PIXEL56	=	D46	,
	PIXEL55	=	D47	,
	PIXEL54	=	D43	,
	PIXEL53	=	D48	,
	PIXEL52	=	D44	,
	PIXEL51	=	D40	,
	PIXEL50	=	D36	,
	PIXEL49	=	D39	,
	PIXEL48	=	D35	,
	PIXEL47	=	D50	,
	PIXEL46	=	D54	,
	PIXEL45	=	D49	,
	PIXEL44	=	D53	,
	PIXEL43	=	D57	,
	PIXEL42	=	D61	,
	PIXEL41	=	D58	,
	PIXEL40	=	D62	,
	PIXEL39	=	D63	,
	PIXEL38	=	D59	,
	PIXEL37	=	D64	,
	PIXEL36	=	D60	,
	PIXEL35	=	D56	,
	PIXEL34	=	D52	,
	PIXEL33	=	D55	,
	PIXEL32	=	D51	,
	PIXEL31	=	D66	,
	PIXEL30	=	D70	,
	PIXEL29	=	D65	,
	PIXEL28	=	D69	,
	PIXEL27	=	D73	,
	PIXEL26	=	D77	,
	PIXEL25	=	D74	,
	PIXEL24	=	D78	,
	PIXEL23	=	D79	,
	PIXEL22	=	D75	,
	PIXEL21	=	D80	,
	PIXEL20	=	D76	,
	PIXEL19	=	D72	,
	PIXEL18	=	D68	,
	PIXEL17	=	D71	,
	PIXEL16	=	D67	,
	PIXEL15	=	D82	,
	PIXEL14	=	D86	,
	PIXEL13	=	D81	,
	PIXEL12	=	D85	,
	PIXEL11	=	D89	,
	PIXEL10	=	D93	,
	PIXEL9	=	D90	,
	PIXEL8	=	D94	,
	PIXEL7	=	D95	,
	PIXEL6	=	D91	,
	PIXEL5	=	D96	,
	PIXEL4	=	D92	,
	PIXEL3	=	D88	,
	PIXEL2	=	D84	,
	PIXEL1	=	D87	,
	PIXEL0	=	D83
}Pixel;


#endif /* LEDMAP_H_ */
