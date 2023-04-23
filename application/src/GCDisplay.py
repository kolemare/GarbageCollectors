import cv2
import time
import os


class ImageWindow:
    def __init__(self, image_path, update_interval):
        self.image_path = image_path
        self.update_interval = update_interval

        self.update_image()

    def update_image(self):
        while True:
            if os.path.exists(self.image_path):
                # Read the image and display it
                try:
                    image = cv2.imread(self.image_path)
                    cv2.imshow("Image Viewer", image)

                    # Wait for a key press or the update interval to pass
                    key = cv2.waitKey(self.update_interval)
                    if key == ord("q"):
                        break
                except:
                    pass
                    # Sleep for the update interval
            time.sleep(self.update_interval / 1000)

        cv2.destroyAllWindows()


def main():
    image_path = "gc_visualizer.png"
    update_interval = 50  # in milliseconds

    app = ImageWindow(image_path, update_interval)


if __name__ == "__main__":
    main()
