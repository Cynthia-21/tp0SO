package ar.edu.utn.frba.dds.servicioagregador.models.dtos.criteriosDePertenencia;

import lombok.Data;
import lombok.Getter;
import lombok.Setter;

@Data
@Getter
@Setter
public class CriterioCategoriaDTO extends CriterioPertenenciaDTO {
  private Long categoriaId;
}
