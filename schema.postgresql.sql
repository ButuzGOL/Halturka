create table customer
(
    customer_id serial,
    name varchar(32) not null,
    surname varchar(32) not null,
    email varchar(64),
    address varchar(64) not null,
    phone varchar(16) not null,
    CONSTRAINT customer_pk PRIMARY KEY(customer_id)
);

create table material
(
    material_id serial,
    work_id integer not null,
    name varchar(32) not null,
    count integer not null,
    price numeric(6, 2) not null,
    CONSTRAINT material_pk PRIMARY KEY(material_id)
);

create table worker
(
    worker_id serial,
    name varchar(32) not null,
    surname varchar(32) not null,
    email varchar(64),
    address varchar(64) not null,
    phone varchar(16) not null,
    CONSTRAINT worker_pk PRIMARY KEY(worker_id)
);

create table payment
(
    payment_id serial,
    customer_id integer REFERENCES customer(customer_id),
    worker_id integer REFERENCES worker(worker_id),
    work_id integer not null,
    date date,
    price numeric(6, 2),
    CONSTRAINT payment_pk PRIMARY KEY(payment_id),
    CONSTRAINT payment_work_id_fk FOREIGN KEY(work_id) REFERENCES work(work_id)
);

create table work
(
    work_id serial,
    customer_id integer not null,
    name varchar(32) not null,
    status boolean,
    date_start date not null,
    date_finish date,
    price numeric(6, 2),
    CONSTRAINT work_pk PRIMARY KEY(work_id),
    CONSTRAINT work_customer_id_fk FOREIGN KEY(customer_id) REFERENCES customer(customer_id)
);

create table work_worker (
    work_worker_id serial,
    work_id integer not null REFERENCES work(work_id),
    worker_id integer not null REFERENCES worker(worker_id),
    CONSTRAINT work_worker_pk PRIMARY KEY(work_worker_id)
);

