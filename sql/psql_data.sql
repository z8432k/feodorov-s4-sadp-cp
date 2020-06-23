--
-- PostgreSQL database dump
--

-- Dumped from database version 12.3 (Ubuntu 12.3-1.pgdg16.04+1)
-- Dumped by pg_dump version 12.3 (Ubuntu 12.3-1.pgdg16.04+1)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- Data for Name: cars; Type: TABLE DATA; Schema: public; Owner: alexander
--

COPY public.cars (id, car_number, model, color, year, exi) FROM stdin;
6	а000аа78	баржа-универсам	ржавый	1960	t
7	р008ее78	трабант	бежевый	1950	t
8	о777ее77	белаз	валентина	1999	t
9	о123тт98	ЗАЗ-969	коричневый	1969	t
10	р987ав78	стамеска	вишнёвый	2000	t
11	р765сс78	икарус	красный	1985	t
\.


--
-- Data for Name: clients; Type: TABLE DATA; Schema: public; Owner: alexander
--

COPY public.clients (id, client_name, license, passport, address) FROM stdin;
5	Боря	78аа123456	рпмсоп	асрпас
8	Братиша	00аа000002	уважаю	дом офицеров
6	Полковник	78аа123457	на белом коне	уважаю
7	Пахом	00аа000001	как цапля	поехавший
\.


--
-- Data for Name: rents; Type: TABLE DATA; Schema: public; Owner: alexander
--

COPY public.rents (id, client_license, car_number, rent_date, return_date) FROM stdin;
21	78аа123456	а000аа78	2020-06-23	2020-06-23
23	78аа123457	а000аа78	2020-06-23	2020-06-23
25	78аа123456	а000аа78	2020-06-23	2020-06-23
26	78аа123457	а000аа78	2020-06-23	2020-06-23
\.


--
-- Name: cars_id_seq; Type: SEQUENCE SET; Schema: public; Owner: alexander
--

SELECT pg_catalog.setval('public.cars_id_seq', 11, true);


--
-- Name: clients_id_seq; Type: SEQUENCE SET; Schema: public; Owner: alexander
--

SELECT pg_catalog.setval('public.clients_id_seq', 8, true);


--
-- Name: rents_id_seq; Type: SEQUENCE SET; Schema: public; Owner: alexander
--

SELECT pg_catalog.setval('public.rents_id_seq', 26, true);


--
-- PostgreSQL database dump complete
--

