import cv2
import time
import os


class ImageWindow:
    def __init__(self, image_paths, update_interval):
        self.gc_image_path = image_paths[0]
        self.heap_image_path = image_paths[1]
        self.update_interval = update_interval

        self.update_image()

    def update_image(self):
        while True:
            if os.path.exists(self.gc_image_path) and os.path.exists(self.heap_image_path):
                # Read the images and display them
                try:
                    gc_image = cv2.imread(self.gc_image_path)
                    heap_image = cv2.imread(self.heap_image_path)

                    cv2.imshow("GC Image Viewer", gc_image)
                    cv2.imshow("Heap Image Viewer", heap_image)

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
    gc_image_path = "gc_visualizer.png"
    heap_image_path = "heap.png"
    image_paths = [gc_image_path, heap_image_path]
    update_interval = 50  # in milliseconds

    app = ImageWindow(image_paths, update_interval)


if __name__ == "__main__":
    main()
