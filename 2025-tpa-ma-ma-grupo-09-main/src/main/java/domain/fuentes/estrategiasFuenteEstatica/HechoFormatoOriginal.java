package domain.fuentes.estrategiasFuenteEstatica;


import com.opencsv.bean.CsvBindByName;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

@Getter@Setter
@NoArgsConstructor
public class HechoFormatoOriginal {

  @CsvBindByName(column = "Título")
  private String titulo;

  @CsvBindByName(column = "Descripción")
  private String descripcion;

  @CsvBindByName(column = "Categoría")
  private String categoria;

  @CsvBindByName(column = "Latitud")
  private String latitud;

  @CsvBindByName(column = "Longitud")
  private String longitud;

  @CsvBindByName(column = "Fecha del hecho")
  private String fechaDelHecho;
}
