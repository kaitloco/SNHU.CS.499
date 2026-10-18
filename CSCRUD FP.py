
from pymongo import MongoClient
from pymongo.errors import PyMongoError


class AnimalShelter:
    """CRUD operations for the AAC animal collection."""

    def __init__(self):
        USER = "aacuser"
        PASS = "Swifyfunf"
        HOST = "localhost"
        PORT = 27017
        DB = "aac"
        COL = "animals"

        self.client = MongoClient(
            f"mongodb://{USER}:{PASS}@{HOST}:{PORT}/?authSource=admin",
            serverSelectionTimeoutMS=5000
        )

        self.database = self.client[DB]
        self.collection = self.database[COL]

    def create(self, data):
        if not isinstance(data, dict) or not data:
            raise ValueError("Data must be a non-empty dictionary.")

        try:
            result = self.collection.insert_one(data)
            return result.acknowledged
        except PyMongoError as exc:
            print(f"Create error: {exc}")
            return False

    def read(self, query):
        if not isinstance(query, dict):
            raise ValueError("Query must be a dictionary.")

        try:
            return list(self.collection.find(query))
        except PyMongoError as exc:
            print(f"Read error: {exc}")
            return []

    def update(self, query, new_values):
        if not isinstance(query, dict) or not isinstance(new_values, dict):
            raise ValueError("Query and update values must be dictionaries.")

        try:
            result = self.collection.update_many(
                query, {"$set": new_values}
            )
            return result.modified_count
        except PyMongoError as exc:
            print(f"Update error: {exc}")
            return 0

    def delete(self, query):
        if not isinstance(query, dict):
            raise ValueError("Query must be a dictionary.")

        try:
            result = self.collection.delete_many(query)
            return result.deleted_count
        except PyMongoError as exc:
            print(f"Delete error: {exc}")
            return 0

    def health_check(self):
        try:
            self.client.admin.command("ping")
            return True
        except PyMongoError as exc:
            print(f"MongoDB connection error: {exc}")
            return False
