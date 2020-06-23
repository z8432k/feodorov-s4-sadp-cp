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
-- Name: hstore; Type: EXTENSION; Schema: -; Owner: -
--

CREATE EXTENSION IF NOT EXISTS hstore WITH SCHEMA public;


--
-- Name: EXTENSION hstore; Type: COMMENT; Schema: -; Owner: -
--

COMMENT ON EXTENSION hstore IS 'data type for storing sets of (key, value) pairs';


--
-- Name: car_service_check_in_rent(); Type: FUNCTION; Schema: public; Owner: -
--

CREATE FUNCTION public.car_service_check_in_rent() RETURNS trigger
    LANGUAGE plpgsql
    AS $$DECLARE
    num              CHARACTER VARYING;
BEGIN

SELECT car_number INTO num FROM rents WHERE car_number = NEW.car_number AND return_date IS NULL;

IF num IS NOT NULL THEN
	RAISE EXCEPTION 'WTF? Car with number % is rented now.', NEW.car_number;
END IF;

RETURN NEW;
END;$$;


--
-- Name: FUNCTION car_service_check_in_rent(); Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON FUNCTION public.car_service_check_in_rent() IS 'Проверяет корректность возврата автомобиля из обслуживания. Возможно автомобиль в аренде.';


--
-- Name: rents_car_check_exi(); Type: FUNCTION; Schema: public; Owner: -
--

CREATE FUNCTION public.rents_car_check_exi() RETURNS trigger
    LANGUAGE plpgsql
    AS $$DECLARE
    is_exi              BOOLEAN;
BEGIN

SELECT exi INTO is_exi FROM cars WHERE car_number = NEW.car_number;

IF NOT is_exi THEN
	RAISE EXCEPTION 'Car % not available for rent.', NEW.car_number;
ELSE
	UPDATE cars SET exi = FALSE WHERE car_number = NEW.car_number;
END IF;


RETURN NEW;
END;
$$;


--
-- Name: FUNCTION rents_car_check_exi(); Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON FUNCTION public.rents_car_check_exi() IS 'Проверяет доступность автомобиля для аренды';


--
-- Name: rents_car_set_exi(); Type: FUNCTION; Schema: public; Owner: -
--

CREATE FUNCTION public.rents_car_set_exi() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
    is_exi              BOOLEAN;
BEGIN

SELECT exi INTO is_exi FROM cars WHERE car_number = NEW.car_number;

IF OLD.return_date IS NULL AND NEW.return_date IS NOT NULL THEN
	IF NOT is_exi THEN
		UPDATE cars SET exi = TRUE WHERE car_number = OLD.car_number;
	ELSE
		RAISE EXCEPTION 'Something is wrong with car %. Car is already available.', NEW.car_number;
	END IF;
END IF;

RETURN NEW;
END;$$;


--
-- Name: FUNCTION rents_car_set_exi(); Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON FUNCTION public.rents_car_set_exi() IS 'Обновляет флаг доступности автомобиля при возврате последнего';


SET default_table_access_method = heap;

--
-- Name: cars; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.cars (
    id integer NOT NULL,
    car_number character varying(8) NOT NULL,
    model character varying(64) NOT NULL,
    color character varying(32) NOT NULL,
    year smallint NOT NULL,
    exi boolean DEFAULT true NOT NULL,
    CONSTRAINT car_number_format CHECK (((car_number)::text ~ '^[авекмнорстух]\d{3}[авекмнорстух]{2}\d{2}$'::text))
);


--
-- Name: TABLE cars; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE public.cars IS 'Реестр автомобилей';


--
-- Name: CONSTRAINT car_number_format ON cars; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON CONSTRAINT car_number_format ON public.cars IS 'Проверка формата гос. номера автомобиля';


--
-- Name: cars_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.cars_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: cars_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.cars_id_seq OWNED BY public.cars.id;


--
-- Name: clients; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.clients (
    id integer NOT NULL,
    client_name character varying(512) NOT NULL,
    license character varying(10) NOT NULL,
    passport character varying(512) NOT NULL,
    address character varying(512) NOT NULL,
    CONSTRAINT client_license_format CHECK (((license)::text ~ '^\d{2}[авекмнорстух]{2}\d{6}$'::text))
);


--
-- Name: CONSTRAINT client_license_format ON clients; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON CONSTRAINT client_license_format ON public.clients IS 'Проверка формата номера водительского удостоверения';


--
-- Name: clients_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.clients_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: clients_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.clients_id_seq OWNED BY public.clients.id;


--
-- Name: rents; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.rents (
    id integer NOT NULL,
    client_license character varying(10) NOT NULL,
    car_number character varying(8) NOT NULL,
    rent_date date DEFAULT CURRENT_DATE NOT NULL,
    return_date date
);


--
-- Name: rents_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.rents_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: rents_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.rents_id_seq OWNED BY public.rents.id;


--
-- Name: cars id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.cars ALTER COLUMN id SET DEFAULT nextval('public.cars_id_seq'::regclass);


--
-- Name: clients id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.clients ALTER COLUMN id SET DEFAULT nextval('public.clients_id_seq'::regclass);


--
-- Name: rents id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.rents ALTER COLUMN id SET DEFAULT nextval('public.rents_id_seq'::regclass);


--
-- Name: cars cars_pk; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.cars
    ADD CONSTRAINT cars_pk PRIMARY KEY (id);


--
-- Name: clients clients_pk; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.clients
    ADD CONSTRAINT clients_pk PRIMARY KEY (id);


--
-- Name: rents rents_pk; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.rents
    ADD CONSTRAINT rents_pk PRIMARY KEY (id);


--
-- Name: rents rents_valid_return_date; Type: CHECK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE public.rents
    ADD CONSTRAINT rents_valid_return_date CHECK (((return_date IS NULL) OR (return_date >= rent_date))) NOT VALID;


--
-- Name: CONSTRAINT rents_valid_return_date ON rents; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON CONSTRAINT rents_valid_return_date ON public.rents IS 'Проверяет корректность даты возврата. Должно быть больше даты сдачи в аренду';


--
-- Name: cars_number_uindex; Type: INDEX; Schema: public; Owner: -
--

CREATE UNIQUE INDEX cars_number_uindex ON public.cars USING btree (car_number);


--
-- Name: clients_license_uindex; Type: INDEX; Schema: public; Owner: -
--

CREATE UNIQUE INDEX clients_license_uindex ON public.clients USING btree (license);


--
-- Name: rents_cars_number_idx; Type: INDEX; Schema: public; Owner: -
--

CREATE INDEX rents_cars_number_idx ON public.rents USING btree (car_number);


--
-- Name: rents_client_license_idx; Type: INDEX; Schema: public; Owner: -
--

CREATE INDEX rents_client_license_idx ON public.rents USING btree (client_license);


--
-- Name: cars cars_check_service; Type: TRIGGER; Schema: public; Owner: -
--

CREATE TRIGGER cars_check_service BEFORE UPDATE ON public.cars FOR EACH ROW EXECUTE FUNCTION public.car_service_check_in_rent();


--
-- Name: TRIGGER cars_check_service ON cars; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TRIGGER cars_check_service ON public.cars IS 'Проверяет связанную таблицу при попытке вернуть автомобиль из ремонта на предмет текущей аренды.';


--
-- Name: rents rents_check_car_exi; Type: TRIGGER; Schema: public; Owner: -
--

CREATE TRIGGER rents_check_car_exi BEFORE INSERT ON public.rents FOR EACH ROW EXECUTE FUNCTION public.rents_car_check_exi();


--
-- Name: TRIGGER rents_check_car_exi ON rents; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TRIGGER rents_check_car_exi ON public.rents IS 'Выполняет проверку доступности автомобиля для сдачи в аренду';


--
-- Name: rents rents_update_car_exi; Type: TRIGGER; Schema: public; Owner: -
--

CREATE TRIGGER rents_update_car_exi AFTER UPDATE ON public.rents FOR EACH ROW EXECUTE FUNCTION public.rents_car_set_exi();


--
-- Name: TRIGGER rents_update_car_exi ON rents; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TRIGGER rents_update_car_exi ON public.rents IS 'Выполняет обновления флага доступности автомобиля';


--
-- Name: rents rents_cars_car_number_fk; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.rents
    ADD CONSTRAINT rents_cars_car_number_fk FOREIGN KEY (car_number) REFERENCES public.cars(car_number) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: rents rents_cars_car_number_fkey; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.rents
    ADD CONSTRAINT rents_cars_car_number_fkey FOREIGN KEY (car_number) REFERENCES public.cars(car_number) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: rents rents_clients_license_fk; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.rents
    ADD CONSTRAINT rents_clients_license_fk FOREIGN KEY (client_license) REFERENCES public.clients(license) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: rents rents_clients_license_fkey; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.rents
    ADD CONSTRAINT rents_clients_license_fkey FOREIGN KEY (client_license) REFERENCES public.clients(license) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- PostgreSQL database dump complete
--

