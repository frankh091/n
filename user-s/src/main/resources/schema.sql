
CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    username VARCHAR(50) NOT NULL,
    password VARCHAR(255) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE user_permissions (
    id SERIAL PRIMARY KEY,
    user_id INT REFERENCES users(id),
    permission VARCHAR(50) NOT NULL
);

CREATE TABLE user_devices (
    id SERIAL PRIMARY KEY,
    user_id INT REFERENCES users(id),
    device_id INT NOT NULL
);

CREATE TABLE user_groups (
    id SERIAL PRIMARY KEY,
    user_id INT REFERENCES users(id),
    group_id INT NOT NULL
);
