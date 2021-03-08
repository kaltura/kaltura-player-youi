package tv.youi.kalturaplayeryoui.model.tracks;

public class ImageTrack {
    public String id;
    public String label;
    public String imageTemplateUrl;

    public int tilesHorizontal;
    public int tilesVertical;

    public long segmentDuration;
    public long presentationTimeOffset;
    public long timeScale;
    public long startNumber;
    public long endNumber;
    public long bitrate;

    public float width;
    public float height;

    public boolean isSelected;


    public ImageTrack(String id,
                      String label,
                      String imageTemplateUrl,
                      int tilesHorizontal,
                      int tilesVertical,
                      long segmentDuration,
                      long presentationTimeOffset,
                      long timeScale,
                      long startNumber,
                      long endNumber,
                       long bitrate,
                      float width,
                      float height,
                      boolean isSelected) {
        this.id = id;
        this.label = label;
        this.imageTemplateUrl = imageTemplateUrl;

        this.tilesVertical = tilesVertical;
        this.tilesHorizontal = tilesHorizontal;

        this.segmentDuration = segmentDuration;
        this.presentationTimeOffset = presentationTimeOffset;
        this.timeScale = timeScale;
        this.startNumber = startNumber;
        this.endNumber = endNumber;

        this.bitrate = bitrate;
        this.width = width;
        this.height = height;
        this.isSelected = isSelected;
    }
}
