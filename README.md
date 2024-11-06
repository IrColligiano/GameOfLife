Thesis in Computer Science: "Database in confidential enviroment"(provisional)
Project Overview
This repository contains the source code and configuration files developed as part of my thesis in computer science. 
The project focuses on building a secure and efficient confidential computing application, confidential-app, deployed within a Kubernetes cluster using Azure AKS.
The aim is to leverage advanced container isolation techniques and Kubernetes network policies to ensure data security and confidentiality within a sensitive computing environment.

Key Components
1. Kubernetes Deployments
Configurations for deploying the confidential-app within a secure Kubernetes environment.
Custom deployment files for MySQL and Python-based web servers, designed with Kata Containers isolation.
YAML files specifying runtime configurations and runc runtime classes.
2. Custom Docker Images
Dockerfiles and image configurations for custom MySQL and Python images, hosted on Azure Container Registry (ACR).
Images include necessary dependencies for secure and efficient application functionality.
3. Network Policies
Configured network policies within the app namespace to restrict ingress and egress traffic, ensuring limited access to the application components.
Policies designed to protect against unauthorized access and maintain data confidentiality.
4. Database and Storage Management
MySQL deployment configured with a PersistentVolumeClaim (PVC) to ensure reliable storage.
Database secrets are securely managed within Kubernetes and integrated with the application for controlled access.
5. Python Application for Database Testing
A Python script developed for querying and testing database interactions with db01.
Useful for monitoring, testing, and validating database connectivity and data integrity.
6. Web Server for HTTPS request
A Ngnix web server for accept the client's requests and forword it to the Python Application.

That's all for now... for now...
